project('raylib-meson', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

# Dépendances Windows
winmm_dep = cc.find_library('winmm')
opengl_dep = cc.find_library('opengl32')
gdi32_dep = cc.find_library('gdi32')
math_dep = cc.find_library('m') # Bibliothèque mathématique

# Raylib
raylib_dep = cc.find_library('raylib', 
  dirs : ['C:/raylib/raylib/src']) # Chemin vers Raylib

# Include directories
inc_raylib = include_directories('C:/raylib/raylib/src')

# Création de l'exécutable
exe = executable('raylib-meson', 
  'raylib_meson.c', 
  dependencies: [
    raylib_dep, #Raylib first to avoid Playsound function conflict with Playsound function from windows
    winmm_dep, 
    opengl_dep, 
    gdi32_dep, 
    math_dep
    
  ],  
  include_directories: inc_raylib
)
