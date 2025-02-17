//===- Syntax.td - TableGen metamodel for syntax::Node hierarchy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The tree representation of the is C++ syntax is quite regular.
//
// There are 4 archetypes of nodes in the syntax tree:
//  - Leaves, owning exactly one token. (syntax::Leaf)
//  - Sequences, with a fixed list of children that should appear in order.
//    The concrete node defines a Role sequence which identifies the children.
//    The type of child in each role is also constrained.
//  - Lists, with children in alternating Element/Delimiter roles. (syntax::List)
//    The concrete node defines the element type, delimiters are always leaves.
//  - Alternatives, where several different node types are allowed.
//    These are modeled as abstract types with inheritance (e.g. Declaration).
//
// This file defines TableGen classes modelling these archetypes.
// The concrete nodes are defined in terms of these classes in Nodes.td.
//
// The C++ classes for the archetypes themselves are written by hand, and the
// concrete node classes will be generated. Migration to TableGen is not
// complete, so currently there is a mix of generated and hand-authored code.
//
//===----------------------------------------------------------------------===//

// Defs derived from NodeType correspond to syntax tree node types.
class NodeType {
  // The NodeType that this node is derived from in the Node class hierarchy.
  NodeType base = ?;
}

// A node type which is defined in Nodes.h rather than by generated code.
// We merely specify the inheritance hierarchy here.
class External<NodeType base_> : NodeType { let base = base_; }

// FIXME: add sequence, list, and alternative archetypes.
