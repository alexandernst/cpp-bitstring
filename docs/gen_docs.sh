#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

doxygen Doxyfile
git checkout gh-pages
mv html/* ../
rm -rf html
