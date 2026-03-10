# CLAUDE.md — blnuhr

Python & Qt5 rendition of the [Berlin Quantity Didactics Clock](http://www.surveyor.in-berlin.de/berlin/uhr/indexe.html).

- **Docs**: http://blnuhr.readthedocs.org
- **Repo**: https://github.com/prjemian/blnuhr/

## Project layout

```
src/blnuhr/
    __init__.py       package metadata and version
    main.py           Clock_blnuhr widget + kopf() entry point
    resources.py      Qt .ui file loader (uses inspect, not __file__)
    resources/
        blnuhr.ui     Qt Designer layout for all clock LEDs
docs/                 Sphinx documentation (leave alone)
arduino-*/            Arduino firmware (leave alone)
circuit/              Fritzing circuit designs (leave alone)
pyproject.toml        build config + setuptools-scm versioning
environment.yml       conda environment
```

## Setup

```bash
conda env create -f environment.yml
conda activate blnuhr
pip install -e .
```

## Run

```bash
blnuhr          # GUI entry point (requires PyQt5 display)
```

## Packaging

- Build system: `setuptools` + `setuptools-scm` (via `pyproject.toml`)
- Version derived from git tags automatically; written to `src/blnuhr/_version.py` at install time (gitignored)
- No `setup.py` or `setup.cfg`; do not recreate them

## Versioning

`setuptools-scm` reads git tags (e.g. `0.1.13`) and writes `src/blnuhr/_version.py`.
To cut a release: `git tag <version> && git push --tags`.

## Scope

Focus on Python source (`src/blnuhr/`) and packaging (`pyproject.toml`, `environment.yml`).
Leave `docs/`, `arduino-*/`, and `circuit/` untouched unless explicitly asked.
