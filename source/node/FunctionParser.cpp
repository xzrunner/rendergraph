#include "rendergraph/node/FunctionParser.h"

#include <lexer/Exception.h>

namespace rendergraph
{
namespace node
{

//////////////////////////////////////////////////////////////////////////
// class FunctionTokenizer
//////////////////////////////////////////////////////////////////////////

FunctionTokenizer::FunctionTokenizer(const std::string& str)
    : lexer::Tokenizer<FunctionToken::Type>(str.c_str(), str.c_str() + str.length(), "\"", '\\')
    , m_skip_eol(true)
{
}

lexer::Tokenizer<FunctionToken::Type>::Token
FunctionTokenizer::EmitToken()
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
                    return Token(FunctionToken::Comment, c, e, Offset(c), start_line, start_column);
                }
                break;
            case '{':
                Advance();
                return Token(FunctionToken::OBrace, c, c+1, Offset(c), start_line, start_column);
            case '}':
                Advance();
                return Token(FunctionToken::CBrace, c, c+1, Offset(c), start_line, start_column);
            case '(':
                Advance();
                return Token(FunctionToken::OParenthesis, c, c+1, Offset(c), start_line, start_column);
            case ')':
                Advance();
                return Token(FunctionToken::CParenthesis, c, c+1, Offset(c), start_line, start_column);
            case '[':
                Advance();
                return Token(FunctionToken::OBracket, c, c+1, Offset(c), start_line, start_column);
            case ']':
                Advance();
                return Token(FunctionToken::CBracket, c, c+1, Offset(c), start_line, start_column);
            case '"': { // quoted string
                Advance();
                c = CurPos();
                const char* e = ReadQuotedString('"', "\n}");
                return Token(FunctionToken::String, c, e, Offset(c), start_line, start_column);
            }
            case '\n':
                if (!m_skip_eol) {
                    Advance();
                    return Token(FunctionToken::Eol, c, c+1, Offset(c), start_line, start_column);
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
					return Token(FunctionToken::Integer, c, e, Offset(c), start_line, start_column);
				}

                e = ReadDecimal(NumberDelim());
				if (e != nullptr) {
					return Token(FunctionToken::Decimal, c, e, Offset(c), start_line, start_column);
				}

                static const std::string str_separator(Whitespace() + "{}();,");
                e = ReadUntil(str_separator);
                if (e == nullptr) {
                    assert(0);
                    throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
                }

                return Token(FunctionToken::String, c, e, Offset(c), start_line, start_column);
            }
        }
    }
    return Token(FunctionToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& FunctionTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ")]},");
    return number_delim;
}

//////////////////////////////////////////////////////////////////////////
// class FunctionParser
//////////////////////////////////////////////////////////////////////////

FunctionParser::FunctionParser(const std::string& str)
    : m_tokenizer(FunctionTokenizer(str))
{
}

void FunctionParser::Parse()
{
    Token token;

    // name
    Expect(FunctionToken::String, token = m_tokenizer.NextToken());
    m_name = token.Data();

    // params
    Expect(FunctionToken::OParenthesis, token = m_tokenizer.NextToken());
    while (token.GetType() != FunctionToken::CParenthesis)
    {
        // in or out
        Expect(FunctionToken::String, token = m_tokenizer.NextToken());
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
    Expect(FunctionToken::CParenthesis, token = m_tokenizer.NextToken());

    // body
    m_body = m_tokenizer.ReadRemainder(FunctionToken::Eof);
}

void FunctionParser::ParseVariable(Variable& var)
{
    Token token;
    Expect(FunctionToken::String, token = m_tokenizer.NextToken());
    auto type = token.Data();
    if (type == "Texture") {
        var.type = VariableType::Texture;
    } else if (type == "RT") {
        var.type = VariableType::RenderTarget;
    } else if (type == "shader") {
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
    } else if (type == "vec1[]") {
        var.type = VariableType::Vec1Array;
    } else if (type == "vec2[]") {
        var.type = VariableType::Vec2Array;
    } else if (type == "vec3[]") {
        var.type = VariableType::Vec3Array;
    } else if (type == "vec4[]") {
        var.type = VariableType::Vec4Array;
    } else {
        assert(0);
    }

    Expect(FunctionToken::String, token = m_tokenizer.NextToken());
    var.name = token.Data();
}

std::map<FunctionToken::Type, std::string> FunctionParser::TokenNames() const
{
	using namespace FunctionToken;

	std::map<FunctionToken::Type, std::string> names;

	return names;
}

}
}