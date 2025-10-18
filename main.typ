#import "@preview/datify:1.0.0": *
#import "@preview/mitex:0.2.5": *

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
#let files = sys.inputs.files.split("\n").map(it => it.split("/"))

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

#let generate_content(files, depth, pref) = {
  let ans = ()
  let groups = group_by(files, it => it.at(0))
  for group in groups {
    if group.len() == 1 and group.at(0).len() == 1 {
      let filename = group.at(0).at(0)
      let path = pref.join("/") + "/" + filename
      let ext = filename.split(".").at(-1)
      let basename = filename.split(".").slice(0, -1).join(".")
      ans.push(heading(eval(basename, mode: "markup"), depth: depth))
      if ext in languages {
        ans.push(raw(read(path), lang: languages.at(ext)))
      } else if ext == "typ" {
        ans.push(eval(read(path), mode: "markup"))
      } else if ext == "tex" {
        ans.push(mitext(read(path)))
      } else if ext == "txt" {
        ans.push(raw(read(path)))
      }
    } else {
      let groupname = group.at(0).at(0)
      pref.push(groupname)
      ans.push(heading(eval(groupname, mode: "markup"), depth: depth))
      for i in range(group.len()) {
        group.at(i).remove(0)
      }
      ans.push(generate_content(group, depth + 1, pref))
      pref.pop()
    }
  }
  return ans.join()
}

#set document(title: title, author: author)
#set page(
  paper: "a4",
  flipped: true,
  margin: (x: 1cm, y: 2cm),
  numbering: "1",
  columns: 3,
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
#set text(font: "Libertinus Serif", lang: "ru", region: "ru", size: 11pt)
#show raw: set text(font: "FantasqueSansM Nerd Font Mono", size: 9pt)
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

#generate_content(files, 1, (content_dir,))
