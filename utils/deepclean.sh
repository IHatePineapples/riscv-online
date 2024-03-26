#!/bin/sh
git clean -fdx -ff

git submodule foreach --recursive git clean -fdx -ff
git submodule foreach --recursive git restore .
