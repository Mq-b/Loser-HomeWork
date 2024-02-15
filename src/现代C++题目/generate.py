from typing import cast
from marko.block import Heading, BlockElement, Document, LinkRefDef
from marko.md_renderer import MarkdownRenderer
from marko.inline import CodeSpan
from marko.parser import Parser as MarkdownParser
import sys
import os

from subprocess import PIPE, Popen
from jinja2 import Environment, FileSystemLoader, select_autoescape

srcdir = os.getcwd()
resdir = os.path.dirname(sys.argv[0])

jinjaenv = Environment(
    loader=FileSystemLoader(os.path.join(resdir, "templates")),
    autoescape=select_autoescape(),
)

SRC = "README.md"


def log(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)


log(f"CWD: {srcdir}")

with open(SRC, "r") as f:
    document = MarkdownParser().parse(f.read())

mdrender = MarkdownRenderer()

log(f"Loaded document {os.path.abspath(SRC)}")
log(f"Got {len(document.children)} children of document root")

sectionstag: list[list[BlockElement]] = []
sectionshtml: list[str] = []


def mit2html(markdown: str):
    popen = Popen(
        ["node", os.path.join(resdir, "mit2html", "mit2html.mjs")],
        stdin=PIPE,
        stdout=PIPE,
    )
    html = popen.communicate(input=markdown.encode("utf-8"), timeout=30.0)
    if html[1]:
        log(html[1].decode("utf-8"))
    if popen.returncode != 0:
        log(f"mit2html failed with code {popen.returncode}")
        if html[0]:
            log(html[0].decode("utf-8"))
        return "<span>!!! ERROR !!!</span>"
    if html[0]:
        return html[0].decode("utf-8")
    return "<span>!!! EMPTY !!!</span>"


sectionid = -1
inregion = False
sectiontag = []

for idx, child in enumerate(document.children):
    # log(f"Procesing child {idx} [{type(child).__name__}:{child.line_number}]: ", end="")
    if isinstance(child, Heading) and child.level == 2:
        # log()
        if inregion:
            log(f"<<<<<<< {sectionid}")
            sectionstag.append(sectiontag)
            sectiontag = []
            inregion = False
        if isinstance(child.children[0], CodeSpan):
            sectionid += 1
            log(f">>>>>>> {sectionid}")
            inregion = True
        log(
            f"{'§'*child.level} {''.join([mdrender.render(element) for element in child.children])}"
        )
    if inregion:
        # log(f'append {sectionid}')
        sectiontag.append(child)
    else:
        # log('skipped')
        pass

if inregion:
    log(f"<<<<<<< {sectionid}")
    sectionstag.append(sectiontag)
    sectiontag = []

lrds: list[LinkRefDef] = [
    element for element in document.children if isinstance(element, LinkRefDef)
]

log(f"Found {len(sectionstag)} sections")
for sectiontag in sectionstag:
    for tag in sectiontag:
        if (
            isinstance(tag, Heading)
            and isinstance(tag.children[0], CodeSpan)
            and tag.level == 2
        ):
            cast(list, tag.children).pop(0)
    sectiontag.extend(lrds)
    pseudo_doc = Document()
    pseudo_doc.link_ref_defs = document.link_ref_defs
    pseudo_doc.children = sectiontag

    mdrender.root_node = None
    sectionmd = mdrender.render(pseudo_doc)
    sectionhtml = mit2html(sectionmd)

    sectionshtml.append(sectionhtml)

template = jinjaenv.get_template("template.html")
with open("generated.html", "w") as f:
    _ = f.write(
        template.render(
            sections=sectionshtml, relresdir=os.path.relpath(resdir, srcdir)
        )
    )
