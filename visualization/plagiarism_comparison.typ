#import "@preview/lilaq:0.6.0" as lq;
#import "@preview/catppuccin:1.1.0": latte
#let base-colors = latte.colors.pairs().filter(((_, color)) => color.accent == false).map(((name, color)) => (name, color.rgb)).to-dict()
#let accent-colors = latte.colors.pairs().filter(((_, color)) => color.accent == true).map(((name, color)) => (name, color.rgb)).to-dict()
#set page(fill: base-colors.base, width: auto, height: auto, margin: 1em);

#let analysis = json("../out/plag2.json")

#let highlight_file(
  file,
  color_fn: weight => gradient
    .linear(
      accent-colors.green.transparentize(60%),
      white.transparentize(100%),
      accent-colors.red.transparentize(60%),
    )
    .sample(50% + calc.abs(weight) * 25%),
) = {
  let file_lines = file.file_content.split("\n")
  let file_symbols = file_lines.map(line => line.split("").slice(1).map(symbol => (0, symbol)))

  for (weight, node) in file.ast_nodes {
    let cur_line = node.range.start_line
    let end_line = node.range.end_line
    let cur_column = node.range.start_column
    let end_column = node.range.end_column

    while cur_line < end_line or (cur_line == end_line and cur_column < end_column) {
      let (old_weight, old_symbol) = file_symbols.at(cur_line).at(cur_column)
      file_symbols.at(cur_line).at(cur_column) = (old_weight + weight, old_symbol)
      cur_column += 1
      if cur_column >= file_lines.at(cur_line).len() {
        cur_line += 1
        cur_column = 0
      }
    }
  }

  let content = file_symbols
    .map((line) => {
        let highlight_mask = range(line.len()).map(_ => false)
        let encountered_non_white = false
        for (i, (_, symbol)) in line.enumerate() {
            if encountered_non_white == false and symbol.find(regex("^\s$")) == none {
                encountered_non_white = true
            }
            if not encountered_non_white {
                highlight_mask.at(i) = true
            }
        }

        return line.zip(highlight_mask).map((((weight, symbol), masked)) =>
        if not masked { highlight(symbol, fill: color_fn(weight)) } else { symbol })
    })
    .intersperse([\ ])
    .flatten()
    .join()
  content
}

#set text(font: "Noto Sans")
#align(center)[
#set text(size: 24pt);
    #let (score, label) = if analysis.plagiarism_score < 0.5 { (1 - analysis.plagiarism_score, text(fill: color.green, "Not Plagiarized")) } else { (analysis.plagiarism_score, text(fill: color.red, "Plagiarized")) };
    Plagiarism Score: 
    
    *#calc.round(score * 100, digits: 2)%* #label
]

#let left_content = highlight_file(analysis.file_1);
#let right_content = highlight_file(analysis.file_2);
#set text(font: "JetBrains Mono");

#let file_comparison(left, right) = {
  grid(
    columns: (auto, auto),
    column-gutter: 1em,
    rows: 1,
    stroke: color.gray.transparentize(50%),
    inset: 5mm,
    fill: white,
    left, right,
  )
}

#file_comparison(left_content, right_content)
