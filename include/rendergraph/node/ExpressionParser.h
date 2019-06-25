#pragma once

#include "rendergraph/Variable.h"

#include <lexer/Tokenizer.h>
#include <lexer/Parser.h>

namespace rg
{
namespace node
{

namespace ExpressionToken
{
	typedef unsigned int Type;

	static const Type Integer       = 1 <<  0; // integer number
	static const Type Decimal       = 1 <<  1; // decimal number
	static const Type String        = 1 <<  2; // string
    static const Type Comment       = 1 <<  3; // line comment starting with ///

	static const Type OParenthesis  = 1 <<  4; // opening parenthesis: (
	static const Type CParenthesis  = 1 <<  5; // closing parenthesis: )
	static const Type OBrace        = 1 <<  6; // opening brace: {
	static const Type CBrace        = 1 <<  7; // closing brace: }
	static const Type OBracket      = 1 <<  8; // opening bracket: [
	static const Type CBracket      = 1 <<  9; // closing bracket: ]

	static const Type Eof           = 1 << 10; // end of file
	static const Type Eol           = 1 << 11; // end of line
}


class ExpressionTokenizer : public lexer::Tokenizer<ExpressionToken::Type>
{
public:
    ExpressionTokenizer(const std::string& str);

    void SetSkipEol(bool skip_eol) {
        m_skip_eol = skip_eol;
    }

protected:
    virtual Token EmitToken() override;

    static const std::string& NumberDelim();

private:
    bool m_skip_eol;

}; // ExpressionTokenizer

class ExpressionParser : public lexer::Parser<ExpressionToken::Type>
{
public:
    ExpressionParser(const std::string& str);

    void Parse();

    auto& GetName() const { return m_name; }

    auto& GetInputs() const { return m_inputs; }
    auto& GetOutputs() const { return m_outputs; }

    auto& GetBody() const { return m_body; }

private:
    void ParseVariable(Variable& var);

    virtual std::map<ExpressionToken::Type, std::string> TokenNames() const override;

private:
    ExpressionTokenizer m_tokenizer;

    std::string m_name;

    std::vector<Variable> m_inputs, m_outputs;

    std::string m_body;

    typedef ExpressionTokenizer::Token Token;

}; // ExpressionParser

}
}