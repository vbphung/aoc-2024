use bit_set::BitSet;
use std::collections::HashSet;

pub fn recur_topo(dag: &[HashSet<usize>], order: &[usize], n: usize) -> Vec<usize> {
    let mut ins = Vec::new();

    let check: BitSet = order.iter().cloned().collect();
    let mut graph: Vec<Vec<usize>> = vec![Vec::new(); n];
    for (u, vs) in dag.iter().enumerate() {
        if !check.contains(u) {
            continue;
        }

        for &v in vs {
            if check.contains(v) {
                graph[u].push(v);
            }
        }
    }

    let mut explored = BitSet::with_capacity(n);
    for &u in order {
        dfs(&graph, &mut explored, &mut ins, u);
    }

    ins
}

fn dfs(graph: &[Vec<usize>], explored: &mut BitSet, ins: &mut Vec<usize>, u: usize) {
    if explored.contains(u) {
        return;
    }

    explored.insert(u);

    if let Some(vs) = graph.get(u) {
        for &v in vs {
            dfs(graph, explored, ins, v);
        }
    }

    ins.push(u);
}
