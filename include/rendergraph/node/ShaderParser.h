#pragma once

#include "rendergraph/Variable.h"

#include <lexer/Tokenizer.h>
#include <lexer/Parser.h>

namespace rg
{
namespace node
{

namespace ShaderToken
{
	typedef unsigned int Type;
	static const Type Integer       = 1 <<  0; // integer number
	static const Type Decimal       = 1 <<  1; // decimal number
	static const Type String        = 1 <<  2; // string
	static const Type OParenthesis  = 1 <<  3; // opening parenthesis: (
	static const Type CParenthesis  = 1 <<  4; // closing parenthesis: )
	static const Type OBrace        = 1 <<  5; // opening brace: {
	static const Type CBrace        = 1 <<  6; // closing brace: }
	static const Type OBracket      = 1 <<  7; // opening bracket: [
	static const Type CBracket      = 1 <<  8; // closing bracket: ]
	static const Type Comment       = 1 <<  9; // line comment starting with ///
	static const Type Eof           = 1 << 10; // end of file
	static const Type Eol           = 1 << 11; // end of line

//    static const Type Semicolon     = 1 << 12;  // end of segment
    static const Type Struct        = 1 << 13;
    static const Type Uniform       = 1 << 14;
}

namespace ShaderFormat
{
	typedef size_t Type;
	static const Type Unknown  = 1 << 0;
	static const Type GLSL     = 1 << 1;
}

class ShaderTokenizer : public lexer::Tokenizer<ShaderToken::Type>
{
public:
    ShaderTokenizer(const std::string& str);

    void SetSkipEol(bool skip_eol) {
        m_skip_eol = skip_eol;
    }

protected:
    virtual Token EmitToken() override;

    static const std::string& NumberDelim();

private:
    bool m_skip_eol;

}; // ShaderTokenizer

class ShaderParser : public lexer::Parser<ShaderToken::Type>
{
public:
    ShaderParser(const std::string& str);

    void Parse();

private:
    void ParseStruct();
    void ParseUniform();

    virtual std::map<ShaderToken::Type, std::string> TokenNames() const override;

private:
    ShaderTokenizer    m_tokenizer;
    ShaderFormat::Type m_format;

    std::vector<Variable> m_uniforms;

    typedef ShaderTokenizer::Token Token;

}; // ShaderParser

}
}