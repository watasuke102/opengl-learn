project(
  'opengl-learn', 'cpp',
  default_options : ['warning_level=3', 'cpp_std=c++20']
)

gl_dep = dependency('gl')
glfw_dep = dependency('glfw3')

executable (
  meson.project_name(),
  [
    'src/main.cpp'
  ],
  include_directories: include_directories('inc'),
  dependencies: [
    gl_dep,
    glfw_dep,
  ]
)
