#!/usr/bin/env bash

cd _build/ \
&& cmake .. \
&& cmake --build . --target install \
&& cd ../_install/ \
&& ./project