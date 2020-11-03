use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, Read, Write};
use std::process::Command;
use std::path::{Path, PathBuf};

mod csv_helper {

  use std::io::{BufRead,BufReader,Read,Write,BufWriter};
  use std::fs::File;
  use std::path::{Path, PathBuf};
  use std::vec::Vec;
  use std::env::temp_dir;

  type Result<T> = std::result::Result<T, Box<dyn std::error::Error>>;

  pub fn read_csv_file(path: &Path, delimiter: &str) -> Result<Vec<Vec<String>>> {
    let file = File::open(path)?;
    let contents = BufReader::new(file).lines().map(|line| {
      // @TODO (lh): not a good use of unwrap here
        line.unwrap().split(delimiter).map(|field| field.to_string()).collect()
    }).collect::<Vec<Vec<_>>>();
    Ok(contents)
  }

  pub fn write_csv_file(path: &Path, delimiter: &str, contents: &Vec<Vec<String>>) -> Result<()> {
    let file = File::create(path)?;
    let mut buf_writer = BufWriter::new(file);

    for line in contents.iter() {
      buf_writer.write(line.join(delimiter).as_ref());
      buf_writer.write("\n".as_ref());
    }
    buf_writer.flush()?;
    Ok(())
  }

  fn test_read_and_write_csv_file() -> () {
    let path = temp_dir().join("test.csv");
    let delimiter = ",";
    let expected = vec![
      vec![ "a".to_string() ],
      vec![ "b".to_string(), "c".to_string() ],
    ];
    write_csv_file(&path, &delimiter, &expected).unwrap();
    let actual = read_csv_file(&path, &delimiter).unwrap();
    assert_eq!(actual, expected);
  }

  pub fn test() {
    test_read_and_write_csv_file();
  }

}

mod metrics {
  use crate::csv_helper;

  pub struct Metrics {

  }

  impl Metrics {
    pub fn test() -> () {
      csv_helper::test();
    }
  }
}

mod datalog {
  use std::collections::VecDeque;

  pub struct Datalog {

  }

  impl Datalog {
    fn decl_of_arity_one_statement(relation: &str, datatype: &str) -> String {
      format!(".decl {r}(a:{d})", r=relation, d=datatype)
    }
    fn decl_of_arity_two_statement(relation: &str, datatype: &str) -> String {
      format!(".decl {r}(a:{d}, b:{d})", r=relation, d=datatype)
    }
    fn decl_of_arity_three_statement(relation: &str, datatype: &str) -> String {
      format!(".decl {r}(a:{d}, b:{d}, c:{d})", r=relation, d=datatype)
    }
    fn input_statement(relation: &str) -> String {
      format!(".input {r}", r=relation)
    }
    fn output_statement(relation: &str) -> String {
      format!(".output {r}", r=relation)
    }
    fn printsize_statement(relation: &str) -> String {
      format!(".printsize {r}", r=relation)
    }
    //
    fn benchmark_lr_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b):-{e}(a,b).{i}(a,b):-{e}(a,x),{i}(x,b).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_rr_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b):-{e}(a,b).{i}(a,b):-{i}(a,x),{e}(x,b).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_nr_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b):-{e}(a,b).{i}(a,b):-{i}(a,x),{i}(x,b).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_sg_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,a):-{e}(a,_);{e}(_,a).{i}(a,b):-{e}(a,x),{e}(b,x),{i}(a,b).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_rsg_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,a):-{e}(a,_);{e}(_,a).{i}(a,b):-{e}(x,a),{e}(x,b),{i}(a,b).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_tc_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b,c):-{e}(a,b),{e}(b,c),{e}(c,a).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_scc_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b):-{e}(a,b),{e}(b,a).", e=edb_relation, i=idb_relation)
    }
    fn benchmark_mn_rules_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b,c):-{e}(a,c),{e}(b,c).", e=edb_relation, i=idb_relation)
    }
    //
    fn special_copy_relation_of_arity_two_statement(edb_relation: &str, idb_relation: &str) -> String {
      format!("{i}(a,b):-{e}(a,b).", e=edb_relation, i=idb_relation)
    }
    fn special_partition_by_modulo_rule_statement(edb_relation: &str, datatype: &str, size: &usize, index: &usize) -> String {
      match datatype {
        "symbol" => format!("{e}_{k}(a,b):-{e}(a,b),ord(a)%{n}={k}.", e=edb_relation, n=size, k=index),
        "number" => format!("{e}_{k}(a,b):-{e}(a,b),a%{n}={k}.", e=edb_relation, n=size, k=index),
            _ => panic!(),
      }
    }
    fn benchmark_nr_idb_rule_statement(idb_relation: &str) -> String {
      format!("{i}(a,b):-{i}(a,x),{i}(x,b).", i=idb_relation)
    }
    //
    fn special_join_algorithm_statement(
      edb_relation: &str, idb_relation: &str,  datatype: &str, split_size: &usize,
      len_fn: fn(usize,usize)->usize,
      inc_fn: fn(usize,usize)->usize,
      cond_fn: fn(usize,usize)->bool,
    ) -> String {
      let mut stmts  = vec![];
      let mut next : VecDeque<usize> = (0..*split_size).collect();
      let mut count = *split_size;
      while next.len() > 1 {
        let mut curr = next;
        next = VecDeque::new();

        let len = len_fn(curr.len(), next.len());
        let inc = inc_fn(curr.len(), next.len());
        while curr.len() > 1 {
          let mut new_i = format!("{i}_{k}", i = idb_relation, k = count);
          stmts.push(Self::decl_of_arity_two_statement(&new_i, datatype));
          next.push_back(count);
          for index in 0..std::cmp::min(len, curr.len()) {
            let mut old_i = format!("{i}_{k}", i = idb_relation, k = curr[index]);
            stmts.push(Self::special_copy_relation_of_arity_two_statement( &old_i, &new_i));
          }
          stmts.push(Self::benchmark_nr_idb_rule_statement(&new_i));
          count = count + 1;
          for _ in 0..std::cmp::min(inc, curr.len()) {
            curr.pop_front();
          }
          if cond_fn(curr.len(), next.len()) {
            break;
          }
        }
        if cond_fn(curr.len(), next.len()) && !curr.is_empty() {
          next.append(&mut curr);
        }
      }
      let tmp = stmts.join("\n");
      let string : &str = &tmp;
        let from = format!("{i}_{k}", i = idb_relation, k = count - 1);
      str::replace(
        string,
        &from,
        idb_relation
      )
    }
    //
    fn benchmark_nr_split_join_rules_statement(edb_relation: &str, idb_relation: &str, datatype: &str, split_size: &usize, join_type: &str) -> String {
      let mut result: Vec<String> = (0..*split_size)
          .map(|k| {
            let e = format!("{e}_{k}", e = edb_relation, k = k);
            let i = format!("{i}_{k}", i = idb_relation, k = k);
            vec![
              Self::decl_of_arity_two_statement(&e, datatype),
              Self::decl_of_arity_two_statement(&i, datatype),
              Self::special_partition_by_modulo_rule_statement(edb_relation, datatype, split_size, &k),
              Self::benchmark_nr_rules_statement(&e, &i),
            ]
          })
          .flatten()
          .collect();
      result
          .push(
            match join_type {
              "complete" => {
                Self::special_join_algorithm_statement(
                  edb_relation, idb_relation, datatype, split_size,
                  |c:usize, n:usize| -> usize { c },
                  |c:usize, n:usize| -> usize { c },
                  |c:usize, n:usize| -> bool { false },
                )
              },
              "lattice" => {
                Self::special_join_algorithm_statement(
                  edb_relation, idb_relation, datatype, split_size,
                  |c:usize, n:usize| -> usize { (c as f64).sqrt().ceil() as usize },
                  |c:usize, n:usize| -> usize { 1 },
                  |c:usize, n:usize| -> bool { false },
                )
              },
              "balanced" => {
                Self::special_join_algorithm_statement(
                  edb_relation, idb_relation, datatype, split_size,
                  |c:usize, n:usize| -> usize { (c as f64).sqrt().ceil() as usize },
                  |c:usize, n:usize| -> usize { (c as f64).sqrt().ceil() as usize },
                  |c:usize, n:usize| -> bool { false },
                )
              }
              "left" => {
                Self::special_join_algorithm_statement(
                  edb_relation, idb_relation, datatype, split_size,
                  |c:usize, n:usize| -> usize { (c as f64).sqrt().ceil() as usize },
                  |c:usize, n:usize| -> usize { (c as f64).sqrt().ceil() as usize },
                  |c:usize, n:usize| -> bool { true },
                )
              }
              _ => "".to_string(),
            }
          );
      result.join("\n")
    }
    //
    pub fn benchmark_statement(benchmark: &str, edb_relation: &str, idb_relation: &str, datatype: &str, split_size: &usize, join_type: &str) -> String {
      let simple_benchmark_of_arity_two = vec![
        Self::decl_of_arity_two_statement(edb_relation, datatype),
        Self::input_statement(edb_relation),
        Self::output_statement(idb_relation),
      ];
      let simple_benchmark_of_arity_three = vec![
        Self::decl_of_arity_two_statement(edb_relation, datatype),
        Self::input_statement(edb_relation),
        Self::decl_of_arity_three_statement(idb_relation, datatype),
        Self::output_statement(idb_relation),
        Self::decl_of_arity_two_statement(idb_relation, datatype),
      ];
      let result = match benchmark {
        "LR" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
          txt.push(Self::benchmark_lr_rules_statement(edb_relation, idb_relation));
          txt
        },
        "RR" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
          txt.push(Self::benchmark_rr_rules_statement(edb_relation, idb_relation));
          txt
        },
        "NR" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          if *split_size < 2 {
            txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
            txt.push(Self::benchmark_nr_rules_statement(edb_relation, idb_relation));
          } else {
            txt.push(Self::benchmark_nr_split_join_rules_statement(edb_relation, idb_relation, datatype, split_size, join_type));
          }
          txt
        },
        "SG" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
          txt.push(Self::benchmark_sg_rules_statement(edb_relation, idb_relation));
          txt
        },
        "RSG" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
          txt.push(Self::benchmark_rsg_rules_statement(edb_relation, idb_relation));
          txt
        },
        "TC" => {
          let mut txt = simple_benchmark_of_arity_three.clone();
          txt.push(Self::decl_of_arity_three_statement(idb_relation, datatype));
          txt.push(Self::benchmark_tc_rules_statement(edb_relation, idb_relation));
          txt
        },
        "SCC" => {
          let mut txt = simple_benchmark_of_arity_two.clone();
          txt.push(Self::decl_of_arity_two_statement(idb_relation, datatype));
          txt.push(Self::benchmark_scc_rules_statement(edb_relation, idb_relation));
          txt
        },
        "MN" => {
          let mut txt = simple_benchmark_of_arity_three.clone();
          txt.push(Self::decl_of_arity_three_statement(idb_relation, datatype));
          txt.push(Self::benchmark_mn_rules_statement(edb_relation, idb_relation));
          txt
        },
        "ALL" => {
          vec![
            Self::decl_of_arity_two_statement(edb_relation, datatype),
            Self::input_statement(edb_relation),
            Self::decl_of_arity_two_statement(idb_relation, datatype),
            Self::output_statement(idb_relation),

            Self::decl_of_arity_two_statement("LR", datatype),
            Self::benchmark_lr_rules_statement(edb_relation, "LR"),

            Self::decl_of_arity_two_statement("RR", datatype),
            Self::benchmark_rr_rules_statement(edb_relation, "RR"),

            Self::decl_of_arity_two_statement("NR", datatype),
            Self::benchmark_nr_rules_statement(edb_relation, "NR"),

            Self::decl_of_arity_two_statement("SG", datatype),
            Self::benchmark_sg_rules_statement(edb_relation, "SG"),

            Self::decl_of_arity_two_statement("RSG", datatype),
            Self::benchmark_rsg_rules_statement(edb_relation, "RSG"),

            /*
            Self::decl_of_arity_three_statement("TC", datatype),
            Self::benchmark_tc_rules_statement(edb_relation, "TC"),
             */

            Self::decl_of_arity_two_statement("SCC", datatype),
            Self::benchmark_scc_rules_statement(edb_relation, "SCC"),

            /*
            Self::decl_of_arity_three_statement("MN", datatype),
            Self::benchmark_mn_rules_statement(edb_relation, "MN"),
            */

            format!(
              // "{i}(a,a):-LR(a,a),RR(a,a),NR(a,a),SG(a,a),RSG(a,a),TC(a,a,a),SCC(a,a),MN(a,a,a).",
              "{i}(a,a):-LR(a,a),RR(a,a),NR(a,a),SG(a,a),RSG(a,a),SCC(a,a).",
              i=idb_relation
            )

          ]
        }
        _ => vec![],
      };
      result.join("\n")
    }

  pub fn benchmark_statement_test() {
    let benchmark = "NR";
    let edb_relation = "E";
    let idb_relation = "I";
    let datatype = "number";
    let split_size = 4;
    let join_type = "lattice";
    let actual = Self::benchmark_statement(benchmark, edb_relation, idb_relation, datatype, &split_size, join_type);
    println!("{}", actual);
  }

  }

  }

fn curl(output: &std::path::Path, url: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("curl")
      .arg("--verbose")
      .arg("--output")
      .arg(output.as_os_str())
      .arg("--url")
      .arg(url)
      .arg("--insecure")
      .status()
}

fn tar_cvzf(cvzf: &str, arg: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("tar")
      .arg("-cvzf")
      .arg(cvzf) // output file
      .arg(arg) // input directory or file
      .status()
}

fn tar_xvzf(xvzf: &str, dir: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("tar")
      .arg("-xvzf")
      .arg(xvzf) // input file
      .arg("-C")
      .arg(dir) // output directory
      .status()
}

fn tar_cvjf(cvjf: &str, arg: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("tar")
      .arg("-cvjf")
      .arg(cvjf) // output file
      .arg(arg) // input directory or file
      .status()
}

fn tar_xvjf(xvjf: &str, dir: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("tar")
      .arg("-xvjf")
      .arg(xvjf) // input file
      .arg("-C")
      .arg(dir) // output directory
      .status()
}

fn gzip_decompress(current_dir: &std::path::Path, arg: &std::path::Path) -> std::io::Result<std::process::ExitStatus> {
  if ! current_dir.exists() {
    std::fs::create_dir_all(current_dir)?;
  }
  assert!(arg.exists());
  Command::new("gzip")
      .current_dir(&current_dir.as_os_str())
      .arg("--keep") // keep files, don't delete
      .arg("--decompress")
      .arg("--verbose")
      .arg(arg)
      .status()
}

fn gzip(current_dir: &std::path::Path, arg: &std::path::Path) -> std::io::Result<std::process::ExitStatus> {
  Command::new("gzip")
      .current_dir(&current_dir.as_os_str())
      .arg("--keep") // keep files, don't delete
      .arg("--verbose")
      .arg(arg)
      .status()
}

fn bzip2_decompress(current_dir: &std::path::Path, arg: &std::path::Path) -> std::io::Result<std::process::ExitStatus> {
  if ! current_dir.exists() {
    std::fs::create_dir_all(current_dir)?;
  }
  assert!(arg.exists());
  Command::new("bzip2")
      .current_dir(&current_dir.as_os_str())
      .arg("--keep") // keep files, don't delete
      .arg("--decompress")
      .arg("--verbose")
      .arg(arg)
      .status()
}

fn bzip2_compress(current_dir: &std::path::Path, arg: &std::path::Path) -> std::io::Result<std::process::ExitStatus> {
  Command::new("bzip2")
      .current_dir(&current_dir.as_os_str())
      .arg("--keep") // keep files, don't delete
      .arg("--compress")
      .arg("--verbose")
      .arg(arg)
      .status()
}

fn zip(arg1: &str, arg2: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("zip")
      .arg(arg1)
      .arg(arg2)
      .status()
}

fn unzip(arg: &str) -> std::io::Result<std::process::ExitStatus> {
  Command::new("unzip")
      .arg(arg)
      .status()
}

fn read_a_file(arg:&std::path::Path) -> std::io::Result<Vec<u8>> {
  let mut file = File::open(arg)?;

  let mut data = Vec::new();
  file.read_to_end(&mut data)?;

  return Ok(data);
}

fn test() -> std::io::Result<()> {

  let tmp_html = std::path::Path::new("tmp.html");

  curl(tmp_html, "http://www.google.com")?;
  let expected = read_a_file(tmp_html)?;

  tar_cvzf("tmp.html.tar.gz", "tmp.html")?;
  std::fs::remove_file("tmp.html")?;
  tar_xvzf("tmp.html.tar.gz", ".")?;
  let mut actual = read_a_file(&tmp_html)?;
  assert_eq!(expected, actual);
  std::fs::remove_file("tmp.html.tar.gz")?;

  tar_cvjf("tmp.html.tar.bz2", "tmp.html")?;
  std::fs::remove_file("tmp.html")?;
  tar_xvjf("tmp.html.tar.bz2", ".")?;
  actual = read_a_file(&tmp_html)?;
  assert_eq!(expected, actual);
  std::fs::remove_file("tmp.html.tar.bz2")?;

  let current_dir = std::path::Path::new(".");

  let tmp_html_gz = std::path::Path::new("tmp.html.gz");

  gzip(&current_dir, &tmp_html)?;
  std::fs::remove_file(&tmp_html)?;
  gzip_decompress(&current_dir, &tmp_html_gz)?;
  actual = read_a_file(&tmp_html)?;
  assert_eq!(expected, actual);
  std::fs::remove_file(&tmp_html_gz)?;

  let tmp_html_bz2 = std::path::Path::new("tmp.html.bz2");

  bzip2_compress(&current_dir, &tmp_html)?;
  std::fs::remove_file(&tmp_html)?;
  bzip2_decompress(&current_dir, &tmp_html_bz2)?;
  actual = read_a_file(&tmp_html)?;
  assert_eq!(expected, actual);
  std::fs::remove_file(&tmp_html_bz2)?;

  zip("tmp.html.zip", "tmp.html")?;
  std::fs::remove_file("tmp.html")?;
  unzip("tmp.html.zip")?;
  actual = read_a_file(&tmp_html)?;
  assert_eq!(expected, actual);
  std::fs::remove_file("tmp.html.zip")?;

  std::fs::remove_file("tmp.html")?;
  return Ok(())
}

#[derive(Debug)]
struct GraphStatistics {
  vertices: usize,
  edges: usize,
  unique_self_loops: usize,
  unique_neighbour_loops: usize,
  duplicate_edges: usize,
  zero_degree_vertices: usize,
  minimum_degree: usize,
  maximum_degree: usize,
  average_degree: f64,
}

impl GraphStatistics {
  pub fn new(graph: &GraphData) -> GraphStatistics {
    let degrees_of_vertices = Self::get_degrees_of_vertices(graph);
    GraphStatistics {
      vertices: Self::get_number_of_vertices(graph),
      edges: Self::get_number_of_edges(graph),
      unique_self_loops: Self::get_number_of_unique_self_loops(graph),
      unique_neighbour_loops: Self::get_number_of_unique_neighbour_loops(graph),
      duplicate_edges: Self::get_number_of_duplicate_edges(graph),
      zero_degree_vertices: degrees_of_vertices.iter().filter(|&x| x.clone() == 0).count(),
      minimum_degree: degrees_of_vertices.iter().min().unwrap().clone(),
      maximum_degree: degrees_of_vertices.iter().max().unwrap().clone(),
      average_degree: degrees_of_vertices.iter().sum::<usize>() as f64 / degrees_of_vertices.len() as f64,
    }
  }
  fn get_number_of_vertices(graph: &GraphData) -> usize {
    graph.vertices.len()
  }
  fn get_number_of_edges(graph: &GraphData) -> usize {
    let mut count = 0;
    for xs in graph.edges.iter() {
      count = count + xs.len();
    }
    count
  }
  fn get_degrees_of_vertices(graph: &GraphData) -> Vec<usize> {
    let mut counts = Vec::new();
    for xs in graph.edges.iter() {
      counts.push(xs.len());
    }
    counts
  }
  fn get_number_of_unique_self_loops(graph: &GraphData) -> usize {
    let mut count = 0;
    for (y, xs) in graph.edges.iter().enumerate() {
      for x in xs.iter() {
        if x.clone() == y {
          count = count + 1;
          // break to avoid counting twice
          break;
        }
      }
    }
    count
  }
  fn get_number_of_unique_neighbour_loops(graph: &GraphData) -> usize {
    let mut count = 0;
    for (y, xs) in graph.edges.iter().enumerate() {
      for x in xs.iter() {
        // use any to obtain first match only
        if graph.edges.get(x.clone()).unwrap().iter().any(|&z| z == y) {
          count = count + 1;
        }
      }
    }
    // divide by two to count nerighbour loop edges as a single edge
    count / 2
  }
  fn get_number_of_duplicate_edges(graph: &GraphData) -> usize {
    let mut count = 0;
    for xs in graph.edges.iter() {
      count = count + { let mut zs = xs.clone(); zs.sort(); zs.dedup(); xs.len() - zs.len() };
    }
    count
  }

}

struct GraphData {
  vertices: HashMap<String, usize>,
  edges: Vec<Vec<usize>>
}

impl GraphData {
  pub fn new() -> GraphData {
    GraphData { vertices: HashMap::new(), edges: Vec::new() }
  }
  pub fn get_or_insert_vertex(self: & mut Self, key: &String) -> usize {
    let old_len = self.vertices.len();
    let value = self.vertices.entry(key.clone()).or_insert(old_len).clone();
    if value == old_len {
      self.edges.push(Vec::new())
    }
    value
  }
  pub fn insert_edge(self: & mut Self, src_value : usize, dest_value: usize) -> () {
    self.edges.get_mut(src_value).unwrap().push(dest_value);
  }
  fn remove_self_loops_and_duplicate_edges(self: & mut Self) -> () {
    self.edges.iter_mut().enumerate().for_each(|(i, xs)| {
      xs.sort(); xs.dedup(); xs.retain(|&x| x != i);
    });
  }
  pub fn read_snap_graph(self: & mut Self, path: &std::path::Path) -> std::io::Result<()> {
    let file = File::open(path)?;
    let buf_reader = std::io::BufReader::new(file);
    for line in buf_reader.lines() {
      if let Ok(line) = line {
        if ! line.starts_with("#") {
          let mut labels = line.split_whitespace();
          if let Some(k1) = labels.next() {
            let v1 = self.get_or_insert_vertex(&String::from(k1));
            for k2 in labels {
              let v2 = self.get_or_insert_vertex(&String::from(k2));
              self.insert_edge(v1, v2);
            }
          }
        }
      }
    }
    self.remove_self_loops_and_duplicate_edges();
    Ok(())
  }
  pub fn read_friendster_graph(self: & mut Self, path: &std::path::Path) -> std::io::Result<()> {
    let file = File::open(path)?;
    let buf_reader = std::io::BufReader::new(file);
    for line in buf_reader.lines() {
      if let Ok(line) = line {
        let mut split_line = line.split(":");
        if let Some(k1) = split_line.next() {
          let vs = split_line.next().unwrap();
          if vs != "private" && vs != "unknown" {
            let v1 = self.get_or_insert_vertex(&String::from(k1));
            for k2 in vs.split(",") {
              let v2 = self.get_or_insert_vertex(&String::from(k2));
              self.insert_edge(v1, v2);
            }
          }
        }
      }
    }
    self.remove_self_loops_and_duplicate_edges();
    Ok(())
  }
  pub fn write_graph(self: & Self, path: &std::path::Path, vertices_file: &str, edges_file: &str, delimiter: &str) -> std::io::Result<()> {
    // write vertices as count of vertices
    let v_file = File::create(path.join(vertices_file))?;
    let mut v_buf_writer = std::io::BufWriter::new(v_file);
    v_buf_writer.write(self.vertices.len().to_string().as_bytes())?;
    v_buf_writer.flush()?;
    // write edges
    let e_file = File::create(path.join(edges_file))?;
    let mut e_buf_writer = std::io::BufWriter::new(e_file);
    for (i, neighbours) in self.edges.iter().enumerate() {
      for j in neighbours.iter() {
        e_buf_writer.write(format!("{}{}{}\n", i, delimiter, j).as_bytes())?;
      }
    }
    e_buf_writer.flush()?;
    Ok(())
  }
  pub fn write_graph_as_facts(self: & Self, path: &std::path::Path) -> std::io::Result<()> {
    self.write_graph(path, "V.facts", "E.facts", "\t")
  }



}
struct Helper {}

type Result<T> = std::result::Result<T, Box<dyn std::error::Error>>;

enum InfoFormat { Snap, XProg, XSocFriendster, XWebYahoo, Synthetic }

struct InfoStruct { name: String, location: String, vertices: usize, edges: usize, directed: bool, format: InfoFormat, }

impl Helper {

  fn run_process_and_write(info: &InfoStruct, root_dir: &Path, graph: &GraphData) -> Result<()> {
    let input_dir = root_dir.join("input").join(&info.name);
    let g_file = input_dir.join(format!("{}.txt", &info.name));
    let v_file = input_dir.join("V.facts");
    let e_file = input_dir.join("E.facts");
    if (!v_file.exists()) || (!e_file.exists()) || (!g_file.exists()) {
      println!("{}: Reading...", &info.name);
      std::fs::create_dir_all(&input_dir)?;
        /*
      {
        println!("{}: Processing...", &info.name);
        let (g, v, e) = { let g = GraphStatistics::new(&graph); (format!("{:?}", g), g.vertices, g.edges) };
        {
          let f = File::create(&g_file)?;
          let mut bw = std::io::BufWriter::new(f);
          bw.write(g.as_bytes())?;
          bw.flush()?;
        }
        {
          println!("{}", g)
        }
        if v != info.vertices { println!("{}: Warning for vertices; actual was {} but expected {}...", &info.name, v, info.vertices); }
        if e != info.edges { println!("{}: Warning for edges; actual was {} but expected {}...", &info.name, e, info.edges); }
      }
      */
      println!("{}: Writing...", &info.name);
      graph.write_graph_as_facts(&input_dir)?;
    }
    Ok(())
  }

  fn run_download_extract_one_gz_txt_file(info: &InfoStruct, root_dir: &Path) -> Result<PathBuf> {
    let download_dir = root_dir.join("data").join(&info.name);
    let download_file = {
      let err =  std::io::Error::new(std::io::ErrorKind::Other, "ERROR!");
      let x = Path::new(&info.location).file_name().ok_or(err)?; download_dir.join(x)
    };
    if ! download_file.exists() {
      println!("{}: Downloading...", &info.name);
      std::fs::create_dir_all(&download_dir)?;
      curl(&download_file, &info.location)?;
    }
    let txt_file = {
      let err =  std::io::Error::new(std::io::ErrorKind::Other, "ERROR!");
      let x = download_file.file_stem().ok_or(err)?; download_dir.join(x)
    };
    if ! txt_file.exists() {
      println!("{}: Extracting...", &info.name);
      gzip_decompress(&download_dir, &download_file)?;
    }
    Ok(txt_file)
  }

  fn run_copy_one_file(info: &InfoStruct, root_dir: &Path) -> Result<PathBuf> {
    let src_file = root_dir.join(&info.location);
    let dest_file = {
      let err =  std::io::Error::new(std::io::ErrorKind::Other, "ERROR!");
      let x = Path::new(&info.location).file_name().ok_or(err)?;
      let y = root_dir.join("data").join(&info.name);
      std::fs::create_dir_all(&y);
      y.join(x)
    };
    if ! dest_file.exists() {
      std::fs::copy(&src_file, &dest_file)?;
    }
    Ok(dest_file)
  }

  fn run_for_snap_dataset(info: &InfoStruct, root_dir: &Path) -> Result<()> {
    println!("{}: Beginning...", &info.name);
    let txt_file = Self::run_download_extract_one_gz_txt_file(info, root_dir)?;
    if ! root_dir.join("input").join(&info.name).join(format!("{}.txt", &info.name)).exists() {
      let mut graph = GraphData::new();
      graph.read_snap_graph(txt_file.as_path())?;
      Self::run_process_and_write(info, root_dir, &graph);
    }
    println!("{}: Ending...", &info.name);
    Ok(())
  }

  fn run_for_prog_dataset(info: &InfoStruct, root_dir: &Path) -> Result<()> {
    println!("{}: Beginning...", &info.name);
    let txt_file = Self::run_copy_one_file(info, root_dir)?;
    if ! root_dir.join("input").join(&info.name).join(format!("{}.txt", &info.name)).exists() {
      let mut graph = GraphData::new();
      graph.read_snap_graph(txt_file.as_path())?;
      Self::run_process_and_write(info, root_dir, &graph);
    }
    println!("{}: Ending...", &info.name);
    Ok(())
  }

  fn run_for_synthetic_dataset(info: &InfoStruct, root_dir: &Path) -> Result<()> {
    println!("{}: Beginning...", &info.name);
    println!("{}: Vertices...", &info.vertices);
    println!("{}: Edges...", &info.edges);
    // s.t. 1 / fraction * complete_graph_edges = info.edges, i.e. we skip every nth
    let fraction = {
      let complete_graph_edges = (info.vertices * (info.vertices - 1)) as usize;
      complete_graph_edges / info.edges
    };
    let mut count = 0;
    if ! root_dir.join("input").join(&info.name).join(format!("{}.txt", &info.name)).exists() {
      let mut graph = GraphData::new();
      for i in 0..info.vertices {
        for j in 0..info.vertices {
          if i != j {
            if count % fraction == 0 {
              let x = graph.get_or_insert_vertex(&i.to_string());
              let y = graph.get_or_insert_vertex(&j.to_string());
              graph.insert_edge(x, y);
            }
            count = count + 1
          }
        }
      }
      Self::run_process_and_write(info, root_dir, &graph);
    }
    println!("{}: Ending...", &info.name);
    Ok(())
  }

  fn run(info: &InfoStruct) -> Result<()> {
    let home_var = std::env::var("HOME")?;
    let home_dir = Path::new(&home_var);
    let root_dir = home_dir.join(".souffle");
    match info.format {
      InfoFormat::Snap => Self::run_for_snap_dataset(info, &root_dir),
      InfoFormat::XProg => Self::run_for_prog_dataset(info, &root_dir),
      InfoFormat::XSocFriendster => {println!("Warning: Not implemented for {}...", info.name); Ok(())},
      InfoFormat::XWebYahoo => {println!("Warning: Not implemented for {}...", info.name); Ok(())},
      InfoFormat::Synthetic => Self::run_for_synthetic_dataset(info, &root_dir),
    }
  }
}

fn make_datasets() -> Result<()> {
  test().expect("ERROR!");
  let mut datasets = vec![
    InfoStruct { name: "cit-Patents".to_string(), location: "https://snap.stanford.edu/data/cit-Patents.txt.gz".to_string(), vertices: 3774768, edges: 16518948, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "com-Orkut".to_string(), location: "https://snap.stanford.edu/data/bigraphata/communities/com-orkut.ungraph.txt.gz".to_string(), vertices: 3072441, edges: 117185083, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "com-Youtube".to_string(), location: "https://snap.stanford.edu/data/bigraphata/communities/com-youtube.ungraph.txt.gz".to_string(), vertices: 1134890, edges: 2987624, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "roadNet-CA".to_string(), location: "https://snap.stanford.edu/data/roadNet-CA.txt.gz".to_string(), vertices: 1965206, edges: 2766607 * 2, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "roadNet-PA".to_string(), location: "https://snap.stanford.edu/data/roadNet-PA.txt.gz".to_string(), vertices: 1088092, edges: 1541898 * 2, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "roadNet-TX".to_string(), location: "https://snap.stanford.edu/data/roadNet-TX.txt.gz".to_string(), vertices: 1379917, edges: 1921660 * 2, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "soc-LiveJournal1".to_string(), location: "https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz".to_string(), vertices: 4847571, edges: 68993773, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "soc-Epinions1".to_string(), location: "https://snap.stanford.edu/data/soc-Epinions1.txt.gz".to_string(), vertices: 75879, edges: 508837, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "soc-Pokec".to_string(), location: "https://snap.stanford.edu/data/soc-pokec-relationships.txt.gz".to_string(), vertices: 1632803, edges: 30622564, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "web-BerkStan".to_string(), location: "https://snap.stanford.edu/data/web-BerkStan.txt.gz".to_string(), vertices: 685230, edges: 7600595, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "web-Google".to_string(), location: "https://snap.stanford.edu/data/web-Google.txt.gz".to_string(), vertices: 875713, edges:  5105039, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "web-NotreDame".to_string(), location: "https://snap.stanford.edu/data/web-NotreDame.txt.gz".to_string(), vertices: 325729, edges: 1497134, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "web-Stanford".to_string(), location: "https://snap.stanford.edu/data/web-Stanford.txt.gz".to_string(), vertices: 281903, edges: 2312497, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "wiki-Talk".to_string(), location: "https://snap.stanford.edu/data/wiki-Talk.txt.gz".to_string(), vertices: 2394385, edges: 5021410, directed: true, format: InfoFormat::Snap, },
    InfoStruct { name: "wiki-topcats".to_string(), location: "https://snap.stanford.edu/data/wiki-topcats.txt.gz".to_string(), vertices: 1791489, edges: 28511807, directed: false, format: InfoFormat::Snap, },
    InfoStruct { name: "prog-jenkins".to_string(), location: "archive/prog-jenkins.csv".to_string(), vertices: 3600863, edges: 59821383, directed: true, format: InfoFormat::XProg, },
    InfoStruct { name: "prog-jython".to_string(), location: "archive/prog-jython.csv".to_string(), vertices: 956295, edges: 26716793, directed: true, format: InfoFormat::XProg, },
    InfoStruct { name: "prog-openjdk8".to_string(), location: "archive/prog-openjdk8.csv".to_string(), vertices: 1789555, edges: 12387187, directed: true, format: InfoFormat::XProg, },
    InfoStruct { name: "example".to_string(), location: "archive/example/E.facts".to_string(), vertices: 8, edges: 8, directed: true, format: InfoFormat::XProg, },
    InfoStruct { name: "x-soc-friendster".to_string(), location: "?".to_string(), vertices: 117751379, edges: 2586147869, directed: true, format: InfoFormat::XSocFriendster, },
    InfoStruct { name: "x-web-yahoo".to_string(), location: "?".to_string(), vertices: 1413511393, edges: 0, directed: true, format: InfoFormat::XWebYahoo, }
  ];
  for i in 1u32..22 {
    datasets.push(
      InfoStruct {
        name: format!("half-complete-graph-{}", 2u64.pow(i)).to_string(),
        location: "none".to_string(),
        vertices: 2u64.pow(i) as usize,
        edges: ((2u64.pow(i) * (2u64.pow(i) - 1)) / 2) as usize,
        directed: true,
        format: InfoFormat::Synthetic,
      },
    )
  }
  /*
  for i in 1u32..13 {
      datasets.push(
          InfoStruct {
            name: format!("complete-graph-{}", 2u64.pow(i)).to_string(),
            location: "none".to_string(),
            vertices: 2u64.pow(i) as usize,
            edges: (2u64.pow(i) * (2u64.pow(i) - 1)) as usize,
            directed: true,
            format: InfoFormat::Synthetic,
          },
      )
  }
   */
  for dataset in datasets.iter() {
    Helper::run(dataset)?;
  }
  Ok(())
}

fn make_benchmark(benchmark:&str, datatype:&str, split_size_str:&str, join_type:&str) -> Result<()> {
  let home_var = std::env::var("HOME")?;
  let home_dir = Path::new(&home_var);
  let root_dir = home_dir.join(".souffle");
  //
  let split_size: usize = split_size_str.parse()?;
  let benchmark_dir = root_dir.join("datalog");
  std::fs::create_dir_all(&benchmark_dir);
  let benchmark_text = datalog::Datalog::benchmark_statement(benchmark, "E", "I", datatype, &split_size, join_type);
  let benchmark_file = benchmark_dir.join(format!("{}_{}_{}_{}.dl", benchmark, datatype, split_size, join_type));
  let f = File::create(&benchmark_file)?;
  let mut bw = std::io::BufWriter::new(f);
  bw.write(benchmark_text.as_bytes())?;
  bw.flush()?;
  Ok(())
}

fn main() {
  let args: Vec<String> = std::env::args().collect();
  match args[1].as_str() {
    "--datasets" => {
      make_datasets().expect("ERROR!");
    }
    "--benchmark" => {
      make_benchmark(
        &args[2],
        &args[3],
        &args[4],
        &args[5]
      ).expect("ERROR!");
    }
    "--metrics" => {
      // @TODO (lh): this command may be used in future to generate the metrics
      metrics::Metrics::test();
    }
    _ => panic!("ERROR!")
  }
}

