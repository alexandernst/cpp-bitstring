#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

doxygen Doxyfile
git checkout gh-pages

#git add -A
#git commit -m "Generate new docs"
