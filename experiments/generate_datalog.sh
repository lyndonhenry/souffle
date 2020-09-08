#!/bin/bash

#
# This script generates Datalog files for the experiments.
#

rm -rf *.dl

cat > LeftLinearTransitiveClosure.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path

Path(a, b) :- 
  Edge(a, b).

Path(a, b) :- 
  Edge(a, x), 
  Path(x, b).

EOF

cat > RightLinearTransitiveClosure.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path

Path(a, b) :- 
  Edge(a, b).

Path(a, b) :- 
  Path(a, x), 
  Edge(x, b).

EOF

cat > NonLinearTransitiveClosure.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path

Path(a, b) :- Edge(a, b).
Path(a, b) :- Path(a, x), Path(x, b).

EOF

cat > ReverseSameGeneration.dl << EOF

.decl Flat(a:number, b:number)
.input Flat
.decl Up(a:number, b:number)
.input Up
.decl Down(a:number, b:number)
.input Down
.decl SameGeneration(a:number, b:number)
.output SameGeneration

SameGeneration(a, b) :- 
  Flat(a, b).

SameGeneration(a, b) :- 
  Up(a, x), 
  SameGeneration(y, x), 
  Down(y, b).

EOF

cat > SameGeneration.dl << EOF

.decl Flat(a:number, b:number)
.input Flat
.decl Up(a:number, b:number)
.input Up
.decl Down(a:number, b:number)
.input Down
.decl SameGeneration(a:number, b:number)
.output SameGeneration

SameGeneration(a, b) :- 
  Flat(a, b).

SameGeneration(a, b) :- 
  Up(a, x), SameGeneration(x, y), Down(y, b).

EOF

cat > TopologicalOrdering.dl << EOF

// Topological Ordering

// Gives a topological ordering over the vertices of an input graph. Note that is works only on input graph is acyclic.

// Graph

// Edges
.decl Edge(v1:number, v2:number)
.input Edge

// Vertices
.decl Vertex(v:number) 
Vertex(v) :- Edge(v,_).
Vertex(v) :- Edge(_,v).

// Tests if vertex v1 proceeds vertex v2 in the ordering.
.decl IsBefore(v1:number, v2:number)

// Tests if vertex v1 succeeds vertex v2 in the ordering.
.decl IsAfter(v1:number, v2:number)

// Gives the possible Indices of the vertices in the ordering.
.decl Indices(v:number, i:number) 

// Gives the exact Index of the vertex in the ordering.
.decl Index(v:number, i:number) 
.output Index

// Vertex x is before vertex y if the graph has an edge from x to y.
IsBefore(x, y) :-
    edge(x, y).

// Vertex x is before vertex y if some vertex z is before x and z is before y.
IsBefore(x, y) :-
    IsBefore(x, z),
    IsBefore(z, y).

// Vertex x is after vertex y if the graph has an edge from y to x.
IsAfter(x, y) :-
    edge(y, x).

// Vertex x is after vertex y if some vertex z is after x and y is after z.
IsAfter(x, y) :-
    IsAfter(z, x),
    IsAfter(y, z).

// The Index of a vertex x is 0 if it has no inbound or outbound edges.
Indices(x, 0) :-
    vertex(x),
    !edge(_, x),
    !edge(x, _).

// The Index of a vertex x is 1 if it has no inbound but only outbound edges.
Indices(x, 1) :-
    vertex(x),
    !edge(_, x),
    edge(x, _).

// The possible Indices of a vertex x are 1 more than the Index of some vertex y if that vertex y is before x but not after x.
Indices(x, i+1) :-
    IsBefore(y, x),
    !IsBefore(x, y),
    IsAfter(x, y),
    Indices(y, i).

// The exact Index of a vertex is the maximum of its possible Indices.
Index(x, i) :-
    Indices(x, i),
    !Indices(x, i+1).

EOF

cat > CallInsensitiveFieldSensitivePointsTo.dl << EOF

.type Variable <: symbol
.type Allocation <: symbol
.type Field <: symbol

.decl AssignAlloc(var:Variable, heap:Allocation)
.output AssignAlloc()
.decl Assign(source:Variable, destination:Variable)
.output Assign()
.decl PrimitiveAssign(source:Variable, dest:Variable)
.output PrimitiveAssign()
.decl Load(base:Variable, dest:Variable, field:Field)
.output Load()
.decl Store(source:Variable, base:Variable, field:Field)
.output Store()
.decl VarPointsTo(var:Variable, heap:Allocation)
.output VarPointsTo()
.decl Alias(x:Variable,y:Variable)
.output Alias()

Assign(var1, var2) :-
  PrimitiveAssign(var1,var2).

Alias(instanceVar, iVar) :-
  VarPointsTo(instanceVar, instanceHeap),
  VarPointsTo(iVar, instanceHeap).

VarPointsTo(var, heap) :-
  AssignAlloc(var, heap).

VarPointsTo(var1, heap) :-
  Assign(var2, var1),
  VarPointsTo(var2, heap).

Assign(var1, var2) :-
  Store(var1, instanceVar2, field),
  Alias(instanceVar2, instanceVar1),
  Load(instanceVar1, var2, field).

EOF
