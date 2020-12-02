# frozen_string_literal: true

require 'mkmf'
require 'rbconfig'

$CFLAGS = '-Wall -Werror -I./webrtc -Wno-unused-function -Wimplicit-function-declaration -g3'
$CXXFLAGS = '-std=c++11 -Wall -Werror -I./miniaudio -g3'

$CC = 'gcc'
$CXX = 'g++'

$srcs = [
  'rb-miniaudio.c'
]

create_makefile 'miniaudio/miniaudio'
