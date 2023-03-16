/*****************************************************************************
 * expr_evaluator.hpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EXPR_EVALUATOR_HPP
#define EXPR_EVALUATOR_HPP

#include "../src/skin_common.hpp"
#include <list>
#include <string>

/// Expression evaluator using Reverse Polish Notation
class ExprEvaluator: public SkinObject
{
public:
    ExprEvaluator( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
    ~ExprEvaluator() { }

    /// Clear the RPN stack and parse an expression
    void parse( const std::string &rExpr );

    /// Pop the first token from the RPN stack.
    /// Return NULL when the stack is empty.
    std::string getToken();

private:
    /// RPN stack
    std::list<std::string> m_stack;

    /// Returns true if op1 has precedency over op2
    bool hasPrecedency( const std::string &op1, const std::string &op2 ) const;
};

#endif
