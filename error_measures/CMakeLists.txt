target_sources(fluidity PRIVATE
	Anisotropic_gradation_metric.F90
	Anisotropic_ZZ.F90
	Aspect_ratios.F90
	Assemble_metric.F90
	Boundary_metric.F90
	Bounding_box_metric.F90
	Conformity_measurement.F90
	Edge_lengths.F90
	Field_preprocessing.F90
	Form_metric.F90
	Geometric_constraints_metric.F90
	Goal_metric.F90
	Goals.F90
	Gradation_metric.F90
	Huang_metric.F90
	Interpolation_error.F90
	Interpolation_metric.F90
	Limit_metric.F90
	Mba_adapt.F90
	Metric_advection.F90
	Project_metric_to_surface.F90
	Recovery_estimator.F90
	Reference_Meshes.F90
	Richardson_metric.F90
)

add_subdirectory(tests)
