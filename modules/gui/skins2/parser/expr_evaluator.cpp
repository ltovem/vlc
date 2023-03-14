/*****************************************************************************
 * expr_evaluator.cpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "expr_evaluator.hpp"


void ExprEvaluator::parse( const std::string &rExpr )
{
    m_stack.clear();

    const char *pString = rExpr.c_str();
    std::list<std::string> opStack;   // operator stack
    std::string token;

    // Tokenize the expression
    int begin = 0, end = 0;
    while( pString[begin] )
    {
        // Find the next significant char in the string
        while( pString[begin] == ' ' )
        {
            begin++;
        }

        if( pString[begin] == '(' )
        {
            // '(' found: push it on the stack and continue
            opStack.push_back( "(" );
            begin++;
        }
        else if( pString[begin] == ')' )
        {
            // ')' found: pop the stack until a '(' is found
            while( !opStack.empty() )
            {
                // Pop the stack
                std::string lastOp = opStack.back();
                opStack.pop_back();
                if( lastOp == "(" )
                {
                    break;
                }
                // Push the operator on the RPN stack
                m_stack.push_back( lastOp );
            }
            begin++;
        }
        else
        {
            // Skip white spaces
            end = begin;
            while( pString[end] && pString[end] != ' ' && pString[end] != ')' )
            {
                end++;
            }
            // Get the next token
            token = rExpr.substr( begin, end - begin );
            begin = end;

            // TODO compare to a set of operators
            if( token == "not" || token == "or" || token == "and" )
            {
                // Pop the operator stack while the operator has a higher
                // precedence than the top of the stack
                while( !opStack.empty() &&
                       hasPrecedency( token, opStack.back() ) )
                {
                    // Pop the stack
                    std::string lastOp = opStack.back();
                    opStack.pop_back();
                    m_stack.push_back( lastOp );
                }
                opStack.push_back( token );
            }
            else
            {
                m_stack.push_back( token );
            }
        }
    }
    // Finish popping the operator stack
    while( !opStack.empty() )
    {
        std::string lastOp = opStack.back();
        opStack.pop_back();
        m_stack.push_back( lastOp );
    }
}


std::string ExprEvaluator::getToken()
{
    if( !m_stack.empty() )
    {
        std::string token = m_stack.front();
        m_stack.pop_front();
        return token;
    }
    return "";
}


bool ExprEvaluator::hasPrecedency( const std::string &op1, const std::string &op2 ) const
{
    // FIXME
    if( op1 == "(" )
    {
        return true;
    }
    if( op1 == "and" )
    {
        return (op2 == "or") || (op2 == "not" );
    }
    if( op1 == "or" )
    {
        return (op2 == "not" );
    }
    return false;
}

