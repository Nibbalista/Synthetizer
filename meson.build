project('raylib-meson', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

winmmdep = cc.find_library('winmm')
raylibdep = cc.find_library('raylib',
               dirs : ['C:/raylib/raylib/src']) # Le répertoire doit contenir libraylib.a
inc_raylib = include_directories('C:/raylib/raylib/src') # Le répertoire doit contenir raylib.h 


exe = executable('raylib-meson', 'raylib_meson.c', 
  dependencies: [winmmdep, raylibdep],  include_directories : inc_raylib
  )