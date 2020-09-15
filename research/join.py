import sys

def join(V, I, J, K, L):
    """
    - INPUTS
      - V: The current total number of vertices.
      - I: The number of iterations per epoch.
      - J: The number of vertices joined per iteration.
      - K: The index increment of the first vertex joined in the current iteration.
      - L: The iteration count decrement to be applied to number of iterations for the next epoch.
    - INTERMEDIATES
      - A: The current active set of vertices.
      - B: The set of vertices to be joined in the current iteration.
      - C: The next active set of vertices.
      - k: The index of the first vertex to be joined in the current iteration.
    - OUTPUTS
      - E: The set of edges as vertex pairs in the final graph.
    """
    E = []
    A = [i for i in range(0, V)]
    while len(A) > 1:
        C = [] 
        k = 0
        for i in range(0, I):
            for j in range(0, J):
                assert(k + j < len(A))
                E.append((A[k + j], V))
            k = k + K
            C.append(V)
            V = V + 1
        if k < len(A) - 1:
            for j in range(k, len(A)):
                C.append(A[j])
        A = C
        I = I - L
    return E

def help():
    print("Usage:")
    print("python3 join.py <V> <I> <J> <K> <L>")

def show(E):
    print("digraph {")
    for (a, b) in E:
        print("{} -> {};".format(a, b))
    print("}")

def main():
    if len(sys.argv) != 6:
        help()
    else:
        E = join(
            int(sys.argv[1]),
            int(sys.argv[2]),
            int(sys.argv[3]),
            int(sys.argv[4]),
            int(sys.argv[5])
        )
    show(E)


if __name__ == '__main__':
    main()
