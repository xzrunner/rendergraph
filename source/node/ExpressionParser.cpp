#include "rendergraph/node/ExpressionParser.h"

#include <lexer/Exception.h>

namespace rg
{
namespace node
{

//////////////////////////////////////////////////////////////////////////
// class ExpressionTokenizer
//////////////////////////////////////////////////////////////////////////

ExpressionTokenizer::ExpressionTokenizer(const std::string& str)
    : lexer::Tokenizer<ExpressionToken::Type>(str, "\"", '\\')
    , m_skip_eol(true)
{
}

lexer::Tokenizer<ExpressionToken::Type>::Token
ExpressionTokenizer::EmitToken()
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
                    return Token(ExpressionToken::Comment, c, e, Offset(c), start_line, start_column);
                }
                break;
            case '{':
                Advance();
                return Token(ExpressionToken::OBrace, c, c+1, Offset(c), start_line, start_column);
            case '}':
                Advance();
                return Token(ExpressionToken::CBrace, c, c+1, Offset(c), start_line, start_column);
            case '(':
                Advance();
                return Token(ExpressionToken::OParenthesis, c, c+1, Offset(c), start_line, start_column);
            case ')':
                Advance();
                return Token(ExpressionToken::CParenthesis, c, c+1, Offset(c), start_line, start_column);
            case '[':
                Advance();
                return Token(ExpressionToken::OBracket, c, c+1, Offset(c), start_line, start_column);
            case ']':
                Advance();
                return Token(ExpressionToken::CBracket, c, c+1, Offset(c), start_line, start_column);
            case '"': { // quoted string
                Advance();
                c = CurPos();
                const char* e = ReadQuotedString('"', "\n}");
                return Token(ExpressionToken::String, c, e, Offset(c), start_line, start_column);
            }
            case '\n':
                if (!m_skip_eol) {
                    Advance();
                    return Token(ExpressionToken::Eol, c, c+1, Offset(c), start_line, start_column);
                }
//                SwitchFallthrough();
            case '\r':
            case ' ':
            case '\t':
            case ';':
            case ',':
            {
                static const std::string whitespace(Whitespace() + ";,");
                DiscardWhile(whitespace);
            }
                break;
            default: { // whitespace, integer, decimal or word
                const char* e = ReadInteger(NumberDelim());
				if (e != nullptr) {
					return Token(ExpressionToken::Integer, c, e, Offset(c), start_line, start_column);
				}

                e = ReadDecimal(NumberDelim());
				if (e != nullptr) {
					return Token(ExpressionToken::Decimal, c, e, Offset(c), start_line, start_column);
				}

                static const std::string str_separator(Whitespace() + "{}();,");
                e = ReadUntil(str_separator);
                if (e == nullptr) {
                    throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
                }

                return Token(ExpressionToken::String, c, e, Offset(c), start_line, start_column);
            }
        }
    }
    return Token(ExpressionToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& ExpressionTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ")]}");
    return number_delim;
}

//////////////////////////////////////////////////////////////////////////
// class ExpressionParser
//////////////////////////////////////////////////////////////////////////

ExpressionParser::ExpressionParser(const std::string& str)
    : m_tokenizer(ExpressionTokenizer(str))
{
}

void ExpressionParser::Parse()
{
    Token token;

    // name
    Expect(ExpressionToken::String, token = m_tokenizer.NextToken());
    m_name = token.Data();

    // params
    Expect(ExpressionToken::OParenthesis, token = m_tokenizer.NextToken());
    while (token.GetType() != ExpressionToken::CParenthesis)
    {
        // in or out
        Expect(ExpressionToken::String, token = m_tokenizer.NextToken());
        auto in_out_str = token.Data();
        assert(in_out_str == "in" || in_out_str == "out");

        // var
        Variable var;
        ParseVariable(var);

        if (in_out_str == "in") {
            m_inputs.push_back(var);
        } else if (in_out_str == "out") {
            m_outputs.push_back(var);
        }

        //m_tokenizer.SkipToken();

        token = m_tokenizer.PeekToken();
    }
    Expect(ExpressionToken::CParenthesis, token = m_tokenizer.NextToken());

    // body
    m_body = m_tokenizer.ReadRemainder(ExpressionToken::Eof);
}

void ExpressionParser::ParseVariable(Variable& var)
{
    Token token;
    Expect(ExpressionToken::String, token = m_tokenizer.NextToken());
    auto type = token.Data();
    if (type == "Texture") {
        var.type = VariableType::Texture;
    } else if (type == "RT") {
        var.type = VariableType::RenderTarget;
    } else if (type == "Shader") {
        var.type = VariableType::Shader;
    } else if (type == "Model") {
        var.type = VariableType::Model;
    } else if (type == "int") {
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
    } else if (type == "vec3[]") {
        var.type = VariableType::Vec3Array;
    } else if (type == "vec4[]") {
        var.type = VariableType::Vec4Array;
    } else {
        assert(0);
    }

    Expect(ExpressionToken::String, token = m_tokenizer.NextToken());
    var.name = token.Data();
}

std::map<ExpressionToken::Type, std::string> ExpressionParser::TokenNames() const
{
	using namespace ExpressionToken;

	std::map<ExpressionToken::Type, std::string> names;

	return names;
}

}
}