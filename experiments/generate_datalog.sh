#!/bin/bash

#
# This script generates Datalog files for the experiments.
#

cd experiments

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
    Edge(x, y).

// Vertex x is before vertex y if some vertex z is before x and z is before y.
IsBefore(x, y) :-
    IsBefore(x, z),
    IsBefore(z, y).

// Vertex x is after vertex y if the graph has an edge from y to x.
IsAfter(x, y) :-
    Edge(y, x).

// Vertex x is after vertex y if some vertex z is after x and y is after z.
IsAfter(x, y) :-
    IsAfter(z, x),
    IsAfter(y, z).

// The Index of a vertex x is 0 if it has no inbound or outbound edges.
Indices(x, 0) :-
    Vertex(x),
    !Edge(_, x),
    !Edge(x, _).

// The Index of a vertex x is 1 if it has no inbound but only outbound edges.
Indices(x, 1) :-
    Vertex(x),
    !Edge(_, x),
    Edge(x, _).

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

.symbol_type Variable
.symbol_type Allocation
.symbol_type Field 

.decl AssignAlloc(var:Variable, heap:Allocation)
.input AssignAlloc()
.decl Assign(source:Variable, destination:Variable)
.output Assign()
.decl PrimitiveAssign(source:Variable, dest:Variable)
.input PrimitiveAssign()
.decl Load(base:Variable, dest:Variable, field:Field)
.input Load()
.decl Store(source:Variable, base:Variable, field:Field)
.input Store()
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

# TODO (lh): these could maybe use splitters and joiners
# TODO (lh): e.g. Splitter(R, n, m, j) = for 0 <= i < n : R_i(x_1, ..., x_m) :- x_j % n == i, R_i(x_1, ..., x_m).
# TODO (lh): e.g. Joiner(R, n, m) = for 0 <= i < n : R(x_1, ..., x_m) :- R_i(x_1, ..., x_m).

cat > LeftLinearTransitiveClosureSpecialFirst.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)
.decl Path_2(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Edge_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Edge_1(a, x), Path_1(x, b).

Path_2(a, b) :- Edge_2(a, b).
Path_2(a, b) :- Edge_2(a, x), Path_2(x, b).

Path(a, b) :- Path_0(a, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_2(a, b).
Path(a, b) :- Edge(a, x), Path(x, b).

EOF

cat > LeftLinearTransitiveClosureSpecialSecond.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)
.decl Path_2(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Edge_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Edge_1(a, x), Path_1(x, b).

Path_2(a, b) :- Edge_2(a, b).
Path_2(a, b) :- Edge_2(a, x), Path_2(x, b).

Path(a, b) :- Path_0(a, b).
Path(a, b) :- Path_0(a, x), Path(x, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_1(a, x), Path(x, b).
Path(a, b) :- Path_2(a, b).
Path(a, b) :- Path_2(a, x), Path(x, b).

EOF


cat > LeftLinearTransitiveClosureSpecialThird.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Edge_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Edge_1(a, x), Path_1(x, b).
Path_1(a, b) :- Path_0(a, b).
Path_1(a, b) :- Path_0(a, x), Path_1(x, b).

Path(a, b) :- Edge_2(a, b).
Path(a, b) :- Edge_2(a, x), Path(x, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_1(a, x), Path(x, b).

EOF


cat > LeftLinearTransitiveClosureSpecialFourth.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Edge_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Edge_1(a, x), Path_1(x, b).

Path(a, b) :- Edge_2(a, b).
Path(a, b) :- Edge_2(a, x), Path(x, b).
Path(a, b) :- Path_0(a, b).
Path(a, b) :- Path_0(a, x), Path(x, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_1(a, x), Path(x, b).

EOF

cat > NonLinearTransitiveClosureSpecialFirst.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)
.decl Path_2(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Path_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Path_1(a, x), Path_1(x, b).

Path_2(a, b) :- Edge_2(a, b).
Path_2(a, b) :- Path_2(a, x), Path_2(x, b).

Path(a, b) :- Path_0(a, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_2(a, b).
Path(a, b) :- Path(a, x), Path(x, b).

EOF

cat > NonLinearTransitiveClosureSpecialSecond.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)
.decl Path_2(a:number, b:number)
.decl Path_01(a:number, b:number)
.decl Path_12(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Path_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Path_1(a, x), Path_1(x, b).

Path_2(a, b) :- Edge_2(a, b).
Path_2(a, b) :- Path_2(a, x), Path_2(x, b).

Path_01(a, b) :- Path_0(a, b).
Path_01(a, b) :- Path_1(a, b).
Path_01(a, b) :- Path_01(a, x), Path_01(x, b).

Path_12(a, b) :- Path_1(a, b).
Path_12(a, b) :- Path_2(a, b).
Path_12(a, b) :- Path_12(a, x), Path_12(x, b).

Path(a, b) :- Path_01(a, b).
Path(a, b) :- Path_12(a, b).
Path(a, b) :- Path(a, x), Path(x, b).

EOF

cat > NonLinearTransitiveClosureSpecialThird.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Path_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Path_0(a, b).
Path_1(a, b) :- Path_1(a, x), Path_1(x, b).

Path(a, b) :- Edge_2(a, b).
Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path(a, x), Path(x, b).

EOF

cat > NonLinearTransitiveClosureSpecialFourth.dl << EOF

.decl Edge(a:number, b:number)
.input Edge
.decl Path(a:number, b:number)
.output Path
.decl Edge_0(a:number, b:number)
.decl Edge_1(a:number, b:number)
.decl Edge_2(a:number, b:number)
.decl Path_0(a:number, b:number)
.decl Path_1(a:number, b:number)
.decl Path_2(a:number, b:number)
.decl Path_01(a:number, b:number)

Edge_0(a, b) :- a % 3 = 0, Edge(a, b).
Edge_1(a, b) :- a % 3 = 1, Edge(a, b).
Edge_2(a, b) :- a % 3 = 2, Edge(a, b).

Path_0(a, b) :- Edge_0(a, b).
Path_0(a, b) :- Path_0(a, x), Path_0(x, b).

Path_1(a, b) :- Edge_1(a, b).
Path_1(a, b) :- Path_1(a, x), Path_1(x, b).

Path_2(a, b) :- Edge_2(a, b).
Path_2(a, b) :- Path_2(a, x), Path_2(x, b).

Path_01(a, b) :- Path_0(a, b).
Path_01(a, b) :- Path_1(a, b).
Path_01(a, b) :- Path_01(a, x), Path_01(x, b).

Path(a, b) :- Path_1(a, b).
Path(a, b) :- Path_2(a, b).
Path(a, b) :- Path_01(a, b).
Path(a, b) :- Path(a, x), Path(x, b).

EOF

rm -rf csv
mkdir -p facts csv

for FILE in *.dl
do
  ../src/souffle -Ffacts -Dcsv -r${FILE}.html ${FILE}
done

cd -
