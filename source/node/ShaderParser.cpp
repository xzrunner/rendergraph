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
    : lexer::Tokenizer<ShaderToken::Type>(str, "\"", '\\')
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
                    Advance();
                    if (CurChar() == '/') {
                        Advance();
                        return Token(ShaderToken::Comment, c, c+3, Offset(c), start_line, start_column);
                    }
                    DiscardUntil("\n\r");
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
            //case ';':
            //    Advance();
            //    return Token(ShaderToken::Semicolon, c, c + 1, Offset(c), start_line, start_column);
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
                DiscardWhile(Whitespace());
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

                e = ReadUntil(Whitespace());
				if (e == nullptr) {
					throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
				}

                if (strncmp(c, "struct", strlen("struct")) == 0) {
                    return Token(ShaderToken::String, c, e, Offset(c), start_line, start_column);
                } else if (strncmp(c, "uniform", strlen("uniform")) == 0) {
                    return Token(ShaderToken::Uniform, c, e, Offset(c), start_line, start_column);
                } else {
                    return Token(ShaderToken::String, c, e, Offset(c), start_line, start_column);
                }
            }
        }
    }
    return Token(ShaderToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& ShaderTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ")");
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
    while (token.GetType() != ShaderToken::Eof)
    {
        auto type = token.GetType();

        if (type == ShaderToken::String) {
            printf("%s\n", token.Data().c_str());
        }
        else {
            int zz = 0;
        }

        if (type == ShaderToken::Uniform) {
            ParseUniform();
        }

        token = m_tokenizer.NextToken();
    }
}

void ShaderParser::ParseStruct()
{

}

void ShaderParser::ParseUniform()
{
    Token token = m_tokenizer.NextToken();
}

std::map<ShaderToken::Type, std::string> ShaderParser::TokenNames() const
{
	using namespace ShaderToken;

	std::map<ShaderToken::Type, std::string> names;
	names[Integer]      = "integer";
	names[Decimal]      = "decimal";
	names[String]       = "string";
	names[OParenthesis] = "'('";
	names[CParenthesis] = "')'";
	names[OBrace]       = "'{'";
	names[CBrace]       = "'}'";
	names[OBracket]     = "'['";
	names[CBracket]     = "']'";
	names[Comment]      = "comment";
	names[Eof]          = "end of file";

	names[Struct]       = "struct";
	names[Uniform]      = "uniform";

	return names;
}

}
}