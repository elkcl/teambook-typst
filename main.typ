#import "@preview/datify:1.3.0": *
#import "@preview/cmarker:0.1.8"
#import "@preview/mitex:0.2.7": mitex

#let config = json(sys.inputs.config_file)
#let title = config.title
#let university = config.university
#let team = config.team
#let members = config.members.sorted(key: it => it.split().at(-1))
#let date = datetime(
  year: config.date.year,
  month: config.date.month,
  day: config.date.day,
)
#let author = team + " (" + members.join(", ") + ")"
#let content_dir = sys.inputs.content_dir
#let mode = sys.inputs.at("mode", default: "tour")
#let tour = mode == "tour"
#let files = sys.inputs.files.split("\n").filter(it => it.len() > 0).map(it => it.split("/"))
#let is_preamble(path) = path.len() == 1 and path.at(0) == "preamble.hpp"
#let preamble_files = files.filter(is_preamble)
#let content_files = files.filter(it => not is_preamble(it))

#let group_by(arr, f) = if arr == () { () } else {
  let state = f(arr.at(0))
  let result = ((arr.at(0),),)
  for v in arr.slice(1, arr.len()) {
    if f(v) != state {
      result.push((v,))
      state = f(v)
    } else {
      result.last().push(v)
    }
  }
  result
}

#let languages = (
  cpp: "cpp",
  hpp: "cpp",
  c: "c",
  h: "c",
  py: "python",
  java: "java",
  kt: "kotlin",
)

#let md_math(src, block: false) = mitex(src, block: block)

// Строки #include "..." нужны компилятору сниппета, в справочник они не входят.
// Системные #include <...> остаются. В разделе «Контест» #include "preamble.hpp" печатается.
#let without_user_includes(src, keep_preamble: false) = {
  let preamble_include = regex("(?m)^#include \"preamble(?:\\.hpp)?\"[ \t]*(?:\r?\n|$)")
  let user_include = regex("(?m)^#include \"[^\"]*\"[ \t]*(?:\r?\n+|$)")
  let mark = "/*__KEEP_PREAMBLE_INCLUDE__*/\n"
  let marked = if keep_preamble { src.replace(preamble_include, mark, count: 100) } else { src }
  let stripped = marked.replace(user_include, "", count: 100)
  if keep_preamble { stripped.replace(mark, "#include \"preamble.hpp\"\n", count: 100) } else { stripped }
}

#let show_raw(path) = {
  let ext = path.split(".").at(-1)
  let src = read(path)
  if ext in ("c", "cpp", "h", "hpp") {
    src = without_user_includes(src, keep_preamble: path.split("/").contains("001_Контест"))
  }
  if ext in languages {
    raw(src, lang: languages.at(ext))
  } else {
    raw(src)
  }
}

#let note_prefix(path) = "md" + path.replace(regex("[^0-9A-Za-zА-Яа-яЁё]+"), "")

#let render_notes(path) = {
  let body = read(path)
  if body.trim() == "" { return none }
  cmarker.render(
    body,
    smart-punctuation: false,
    math: md_math,
    h1-level: 2,
    raw-typst: false,
    label-prefix: note_prefix(path),
  )
}

#let is_entry_file(name) = name == "notes.md" or name.starts-with("code.")
#let strip_order_prefix(name) = name.replace(regex("^[0-9]{3}_"), "")

#let render_algorithm(title, group, depth, pref) = {
  let base = pref.join("/") + "/" + title
  let code_path = none
  let notes_path = none
  for item in group {
    let filename = item.at(1)
    let path = base + "/" + filename
    if filename == "notes.md" {
      notes_path = path
    } else {
      code_path = path
    }
  }
  let parts = (heading(eval(strip_order_prefix(title), mode: "markup"), depth: depth),)
  let want_notes = mode == "book" or code_path == none
  let note = if want_notes and notes_path != none { render_notes(notes_path) } else { none }
  if note != none {
    parts.push(note)
    if code_path != none {
      parts.push(v(0.6em))
    }
  }
  if code_path != none {
    parts.push(show_raw(code_path))
  }
  return parts.join()
}

#let generate_content(entries, depth, pref, start_new_page: true) = {
  let ans = ()
  let new_page = start_new_page
  let groups = group_by(entries, it => it.at(0))
  for group in groups {
    let name = group.at(0).at(0)
    if group.all(it => it.len() == 1) {
      for item in group {
        let filename = item.at(0)
        if filename == "preamble.hpp" {
          let path = pref.join("/") + "/" + filename
          ans.push(heading(depth: depth)[Preamble])
          ans.push(show_raw(path))
        }
      }
    } else if group.all(it => it.len() == 2 and is_entry_file(it.at(1))) {
      if mode == "book" and new_page {
        ans.push(pagebreak(weak: true))
      }
      ans.push(render_algorithm(name, group, depth, pref))
      new_page = true
    } else {
      if mode == "book" and new_page {
        ans.push(pagebreak(weak: true))
      }
      pref.push(name)
      ans.push(heading(eval(strip_order_prefix(name), mode: "markup"), depth: depth))
      for i in range(group.len()) {
        let _ = group.at(i).remove(0)
      }
      ans.push(generate_content(group, depth + 1, pref, start_new_page: false))
      let _ = pref.pop()
      new_page = true
    }
  }
  return ans.join()
}

#set document(title: title, author: author)
#set page(
  paper: "a4",
  flipped: tour,
  margin: if tour { (x: 1cm, y: 2cm) } else { (x: 1.8cm, y: 1.8cm) },
  numbering: "1",
  columns: if tour { 3 } else { 1 },
  header: context {
    let pg = counter(page).get().first()
    if pg != 1 {
      align(horizon)[
        #{ university + " (" + members.map(it => it.split().at(-1)).join(", ") + ")" }
        #h(1fr)
        #pg
      ]
    }
  },
  footer: context {
    let pg = counter(page).get().first()
    if pg == 1 {
      align(horizon + center, str(pg))
    }
  },
)
#set text(font: "Libertinus Serif", lang: "ru", region: "ru", size: if tour { 11pt } else { 12pt })
#show raw: set text(
  font: ("FantasqueSansM Nerd Font Mono", "DejaVu Sans Mono"),
  size: if tour { 9pt } else { 10pt },
  ligatures: false,
  features: ("calt": 0),
)
#set heading(numbering: "1.1")
#show outline.entry.where(level: 1): set text(weight: "bold")

#place(
  top + center,
  scope: "parent",
  float: true,
  [
    #set text(size: 25pt)
    #title

    #set text(size: 14pt)
    #author

    #set text(size: 12pt)
    #custom-date-format(date, lang: "ru", pattern: "d MMMM y")
  ],
)

#outline(depth: 5, indent: auto)

#if preamble_files.len() > 0 {
  heading(depth: 1)[Preamble]
  show_raw(content_dir + "/preamble.hpp")
}

#generate_content(content_files, 1, (content_dir,))
