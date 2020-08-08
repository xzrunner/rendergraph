#include "rendergraph/node/ShaderParser.h"

#include <lexer/Exception.h>

namespace
{

rendergraph::VariableType parser_type(const std::string& type)
{
    if (type == "int") {
        return rendergraph::VariableType::Int;
    } else if (type == "bool") {
        return rendergraph::VariableType::Bool;
    } else if (type == "float") {
        return rendergraph::VariableType::Vector1;
    } else if (type == "vec2") {
        return rendergraph::VariableType::Vector2;
    } else if (type == "vec3") {
        return rendergraph::VariableType::Vector3;
    } else if (type == "vec4") {
        return rendergraph::VariableType::Vector4;
    } else if (type == "mat2") {
        return rendergraph::VariableType::Matrix2;
    } else if (type == "mat3") {
        return rendergraph::VariableType::Matrix3;
    } else if (type == "mat4") {
        return rendergraph::VariableType::Matrix4;
    } else if (type == "sampler2D") {
        return rendergraph::VariableType::Sampler2D;
    } else if (type == "samplerCube") {
        return rendergraph::VariableType::SamplerCube;
    } else {
        return rendergraph::VariableType::UserType;
    }
}

}

namespace rendergraph
{
namespace node
{

//////////////////////////////////////////////////////////////////////////
// class ShaderTokenizer
//////////////////////////////////////////////////////////////////////////

ShaderTokenizer::ShaderTokenizer(const std::string& str)
    : lexer::Tokenizer<ShaderToken::Type>(str.c_str(), str.c_str() + str.length(), "\"", '\\')
    , m_skip_eol(true)
{
}

lexer::Tokenizer<ShaderToken::Type>::Token
ShaderTokenizer::EmitToken()
{
    while (!Eof())
	{
        size_t start_line   = Line();
        size_t start_column = Column();
        const char* c = CurPos();
        switch (*c)
		{
            case '/':
                Advance();
                if (CurChar() == '/')
				{
                    const char* e = DiscardUntil("\n\r");
                    return Token(ShaderToken::Comment, c, e, Offset(c), start_line, start_column);
                }
                else
                {
                    return Token(ShaderToken::Div, c, c + 1, Offset(c), start_line, start_column);
                }
                break;
            case '{':
                Advance();
                return Token(ShaderToken::OBrace, c, c+1, Offset(c), start_line, start_column);
            case '}':
                Advance();
                return Token(ShaderToken::CBrace, c, c+1, Offset(c), start_line, start_column);
            case '(':
                Advance();
                return Token(ShaderToken::OParenthesis, c, c+1, Offset(c), start_line, start_column);
            case ')':
                Advance();
                return Token(ShaderToken::CParenthesis, c, c+1, Offset(c), start_line, start_column);
            case '[':
                Advance();
                return Token(ShaderToken::OBracket, c, c+1, Offset(c), start_line, start_column);
            case ']':
                Advance();
                return Token(ShaderToken::CBracket, c, c+1, Offset(c), start_line, start_column);
            case ',':
                Advance();
                return Token(ShaderToken::Comma, c, c + 1, Offset(c), start_line, start_column);
            case '.':
                Advance();
                return Token(ShaderToken::Dot, c, c + 1, Offset(c), start_line, start_column);
            case '#':
            {
                const char* e = DiscardUntil("\n\r");
                return Token(ShaderToken::Pound, c, e, Offset(c), start_line, start_column);
            }
            case ':':
                Advance();
                return Token(ShaderToken::Colon, c, c + 1, Offset(c), start_line, start_column);
            case '?':
                Advance();
                return Token(ShaderToken::Question, c, c + 1, Offset(c), start_line, start_column);
            case '+':
                Advance();
                return Token(ShaderToken::Add, c, c + 1, Offset(c), start_line, start_column);
            case '-':
                Advance();
                return Token(ShaderToken::Sub, c, c + 1, Offset(c), start_line, start_column);
            case '*':
                Advance();
                return Token(ShaderToken::Mul, c, c + 1, Offset(c), start_line, start_column);
            case '=':
                Advance();
                return Token(ShaderToken::Equal, c, c + 1, Offset(c), start_line, start_column);
            case '"': { // quoted string
                Advance();
                c = CurPos();
                const char* e = ReadQuotedString('"', "\n}");
                return Token(ShaderToken::String, c, e, Offset(c), start_line, start_column);
            }
            case '\n':
                if (!m_skip_eol) {
                    Advance();
                    return Token(ShaderToken::Eol, c, c+1, Offset(c), start_line, start_column);
                }
//                SwitchFallthrough();
            case '\r':
            case ' ':
            case '\t':
            case ';':
            {
                static const std::string whitespace(Whitespace() + ";");
                DiscardWhile(whitespace);
            }
                break;
            default: { // whitespace, integer, decimal or word
                const char* e = ReadInteger(NumberDelim());
				if (e != nullptr) {
					return Token(ShaderToken::Integer, c, e, Offset(c), start_line, start_column);
				}

                e = ReadDecimal(NumberDelim());
				if (e != nullptr) {
					return Token(ShaderToken::Decimal, c, e, Offset(c), start_line, start_column);
				}

                static const std::string str_separator(Whitespace() + ";{}[](),.#:?+-*/=");
                e = ReadUntil(str_separator);
				if (e == nullptr) {
                    assert(0);
					throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
				}

                return Token(ShaderToken::String, c, e, Offset(c), start_line, start_column);
            }
        }
    }
    return Token(ShaderToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& ShaderTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ")]}");
    return number_delim;
}

//////////////////////////////////////////////////////////////////////////
// class ShaderParser
//////////////////////////////////////////////////////////////////////////

ShaderParser::ShaderParser(const std::string& str)
    : m_tokenizer(ShaderTokenizer(str))
    , m_format(ShaderFormat::GLSL)
{
}

void ShaderParser::Parse()
{
    Token token = m_tokenizer.PeekToken();
    ShaderToken::Type token_type = token.GetType();
    while (token_type != ShaderToken::Eof)
    {
        bool advanced = false;
        if (token_type == ShaderToken::String)
        {
            auto str = token.Data();
            if (str == "struct") {
                ParseStruct();
                advanced = true;
            } else if (str == "uniform") {
                ParseUniform();
                advanced = true;
            }
        }

        //switch (token_type)
        //{
        //case ShaderToken::String:
        //    printf("[str] %s\n", token.Data().c_str());
        //    break;
        //case ShaderToken::Comment:
        //    printf("[\\] %s\n", token.Data().c_str());
        //    break;
        //case ShaderToken::Pound:
        //    printf("[#] %s\n", token.Data().c_str());
        //    break;
        //}

        if (!advanced) {
            m_tokenizer.NextToken();
        }

        token = m_tokenizer.PeekToken();
        token_type = token.GetType();
    }
}

void ShaderParser::ParseStruct()
{
    Struct s;

    Token token;

    // struct
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    assert(token.Data() == "struct");

    // name
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    s.name = token.Data();

    // members
    Expect(ShaderToken::OBrace, token = m_tokenizer.NextToken());
    token = m_tokenizer.PeekToken();
    while (token.GetType() != ShaderToken::CBrace)
    {
        std::vector<Variable> vars;
        token = ParseVariables(vars);
        std::copy(vars.begin(), vars.end(), std::back_inserter(s.vars));
    }

    m_structs.push_back(s);
}

void ShaderParser::ParseUniform()
{
    Token token;
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    assert(token.Data() == "uniform");

    std::vector<Variable> vars;
    ParseVariables(vars);
    std::copy(vars.begin(), vars.end(), std::back_inserter(m_uniforms));
}

ShaderTokenizer::Token 
ShaderParser::ParseVariables(std::vector<Variable>& vars) const
{
    Token token;
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    auto type_str = token.Data();
    auto type = parser_type(type_str);

    token = m_tokenizer.NextToken();
    switch (token.GetType())
    {
    case ShaderToken::String:
    {
        auto name = token.Data();
        if (type == VariableType::UserType)
        {
            const Struct* find = nullptr;
            for (auto& s : m_structs) {
                if (s.name == type_str) {
                    find = &s;
                    break;
                }
            }
            if (find)
            {
                for (auto& m : find->vars)
                {
                    Variable cvar = m;
                    cvar.user_type = type_str;
                    vars.push_back(cvar);
                }
            }
        }
        else
        {
            Variable var;
            var.type = type;
            var.name = name;
            vars.push_back(var);
        }
    }
        break;
    case ShaderToken::OBrace:
    {
        std::vector<Variable> sub_vars;
        while (m_tokenizer.PeekToken().GetType() != ShaderToken::CBrace)
        {
            std::vector<Variable> _vars;
            ParseVariables(_vars);
            std::copy(_vars.begin(), _vars.end(), std::back_inserter(sub_vars));
        }
        token = m_tokenizer.NextToken();    // skip CBrace
        token = m_tokenizer.PeekToken();
        if (token.GetType() == ShaderToken::String) 
        {
            auto name = token.Data();
            for (auto& var : sub_vars) {
                var.user_type = name;
            }
            std::copy(sub_vars.begin(), sub_vars.end(), std::back_inserter(vars));
        } 
        else 
        {
            Struct s;
            s.name = type_str;
            s.vars = sub_vars;
        }
    }
        break;
    case ShaderToken::OBracket:
    {
        Variable var;
        Expect(ShaderToken::Integer, token = m_tokenizer.NextToken());
        var.count = token.ToInteger<int>();
        Expect(ShaderToken::CBracket, token = m_tokenizer.NextToken());
        vars.push_back(var);
    }
        break;
    default:
        assert(0);
    }

    return token;
}

std::map<ShaderToken::Type, std::string> ShaderParser::TokenNames() const
{
	using namespace ShaderToken;

	std::map<ShaderToken::Type, std::string> names;
	//names[Integer]      = "integer";
	//names[Decimal]      = "decimal";
	//names[String]       = "string";
	//names[OParenthesis] = "'('";
	//names[CParenthesis] = "')'";
	//names[OBrace]       = "'{'";
	//names[CBrace]       = "'}'";
	//names[OBracket]     = "'['";
	//names[CBracket]     = "']'";
 //   names[Comma]        = "','";
 //   names[Dot]          = "'.'";
	//names[Comment]      = "comment";
	//names[Eof]          = "end of file";

	return names;
}

}
}