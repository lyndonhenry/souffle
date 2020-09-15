#!/usr/bin/env bash

set -ouex pipefail

function main() {

  local CMD="python3 join.py"

  # examples

  # left join, 4 vertices
    
  ${CMD} 4 1 2 2 0 | dot -Tpng > left-4.png

  # balanced join, 4 vertices

  ${CMD} 4 2 2 2 1 | dot -Tpng > balanced-4.png

  # complete join, 4 vertices

  ${CMD} 4 1 4 4 0 | dot -Tpng > complete-4.png

  # lattice join, 4 vertices

  ${CMD} 4 3 2 1 1 | dot -Tpng > lattice-4.png

}

main
