configure_file(testharness.py ${CMAKE_CURRENT_BINARY_DIR}/testharness.py)

set(python_source_files
  clean_mayavi_mesh.py
  create_aligned_mesh.py
  create_makefile.py
  create_param_sweep.py
  edge_length_distribution.py
  encode.sh
  fluidity_gid_template.sh
  genpvd.py
  genpvtu.py
  gen_square_meshes.py
  gmsh2triangle.py
  gmsh_mesh_transform.py
  interval.py
  linear_interpolation.py
  make_check_options.py
  make_diagnostic_fields.py
  make_register_diagnostics.py
  mean_flow.py
  mms_tracer_error.py
  nodecount.py
  optimality.py
  pbswrapper.py
  profiling-graph.py
  pvtu2vtu.py
  radial_scale.py
  rename_checkpoint.py
  silenteval.sh
  sphere2plane.py
  stat2csv.py
  statplot.py
  ${CMAKE_CURRENT_BINARY_DIR}/testharness.py
  transform_mesh.py
  triangle2gmsh.py
  unittestharness.py
  update_options.py
  vtu2ensight.py
  vtudecomp.py
  vtudiff.py
)

foreach(python_source_file ${python_source_files})
    get_filename_component(name_we ${python_source_file} NAME_WE)
    configure_file(${python_source_file} ../bin/${name_we} COPYONLY)
endforeach()

set(tools_source_files
  Checkmesh
  Differentiate_Vtu
  Fladapt
  fldiagnostics
  Flredecomp
  gmsh2vtu
  Meshconv
  periodise
  petsc_readnsolve
  Probe_Vtu
  project_to_continuous
  Project_Vtu
  Streamfunction_2D
  Supermesh_Difference
  test_laplacian
  test_pressure_solve
  triangle2vtu
  unifiedmesh
  Vertical_Integration
  visualise_elements
  vtkdiagnostic
  vtkprojection
  vtu2gmsh
  vtu2triangle
  Vtu_Bins
)

foreach(tool ${tools_source_files})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${tool}.F90
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${tool}_main.cpp
   )
    add_executable(${tool}_tool ${tool}.F90 ${tool}_main.cpp)
  elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${tool}.F90)
    add_executable(${tool}_tool ${tool}.F90)
  else()
    add_executable(${tool}_tool ${tool}.cpp)
  endif()
  string(TOLOWER ${tool} tool_name)
  set_target_properties(${tool}_tool PROPERTIES OUTPUT_NAME ${tool_name})
  target_link_libraries(${tool}_tool fluidity)
endforeach()
