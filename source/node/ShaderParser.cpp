#include "rendergraph/node/ShaderParser.h"

#include <lexer/Exception.h>

namespace rg
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
        Variable var;
        ParseVariable(var);
        s.vars.push_back(var);

        token = m_tokenizer.PeekToken();
    }

    m_structs.push_back(s);
}

void ShaderParser::ParseUniform()
{
    // uniform
    Token token;
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    assert(token.Data() == "uniform");

    Variable var;
    ParseVariable(var);
    if (var.type == VariableType::UserType)
    {
        Struct* find = nullptr;
        for (auto& s : m_structs) {
            if (s.name == var.user_type) {
                find = &s;
                break;
            }
        }
        if (find)
        {
            for (auto& m : find->vars)
            {
                Variable cvar = m;
                cvar.user_type = var.name;
                cvar.count     = var.count;
                m_uniforms.push_back(cvar);
            }
        }
    }
    else
    {
        m_uniforms.push_back(var);
    }
}

void ShaderParser::ParseVariable(Variable& var)
{
    Token token;
    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    auto type = token.Data();
    if (type == "int") {
        var.type = VariableType::Int;
    } else if (type == "bool") {
        var.type = VariableType::Bool;
    } else if (type == "float") {
        var.type = VariableType::Vector1;
    } else if (type == "vec2") {
        var.type = VariableType::Vector2;
    } else if (type == "vec3") {
        var.type = VariableType::Vector3;
    } else if (type == "vec4") {
        var.type = VariableType::Vector4;
    } else if (type == "mat2") {
        var.type = VariableType::Matrix2;
    } else if (type == "mat3") {
        var.type = VariableType::Matrix3;
    } else if (type == "mat4") {
        var.type = VariableType::Matrix4;
    } else if (type == "sampler2D") {
        var.type = VariableType::Sampler2D;
    } else if (type == "samplerCube") {
        var.type = VariableType::SamplerCube;
    } else {
        var.type = VariableType::UserType;
        var.user_type = type;
    }

    Expect(ShaderToken::String, token = m_tokenizer.NextToken());
    var.name = token.Data();

    // array
    token = m_tokenizer.PeekToken();
    if (token.GetType() == ShaderToken::OBracket)
    {
        m_tokenizer.NextToken();    // skip open bracket
        Expect(ShaderToken::Integer, token = m_tokenizer.NextToken());
        var.count = token.ToInteger<int>();
        Expect(ShaderToken::CBracket, token = m_tokenizer.NextToken());
    }
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