#include "librr_intracellular.h"

SBMLIntracellular::SBMLIntracellular() : Intracellular()
{
	type = "sbml";
	initial_values.clear();
	// mutations.clear();
	parameters.clear();
}

SBMLIntracellular::SBMLIntracellular(pugi::xml_node& node)
{
	type = "sbml";
	initialize_intracellular_from_pugixml(node);
}

SBMLIntracellular::SBMLIntracellular(SBMLIntracellular* copy) 
{
	type = copy->type;
	// bnd_filename = copy->bnd_filename;
	// cfg_filename = copy->cfg_filename;
	time_step = copy->time_step;
	discrete_time = copy->discrete_time;
	time_tick = copy->time_tick;
	scaling = copy->scaling;
	initial_values = copy->initial_values;
	// mutations = copy->mutations;
	parameters = copy->parameters;
	
	// if (copy->maboss.has_init()) {
	// if (copy->rrHandle.has_init()) {
	// 	// maboss.init_maboss(copy->bnd_filename, copy->cfg_filename);
	// 	// maboss.mutate(mutations);

	// 	rrHandle.set_initial_values(initial_values);
	// 	rrHandle.set_parameters(parameters);
	// 	rrHandle.set_update_time_step(copy->time_step);
	// 	rrHandle.set_discrete_time(copy->discrete_time, copy->time_tick);
	// 	rrHandle.set_scaling(copy->scaling);

	// 	// maboss.restart_node_values();
	// 	//maboss.set_state(copy->maboss.get_maboss_state());
	// 	//std::cout << get_state();
	// }	
}

void SBMLIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	// pugi::xml_node node_bnd = node.child( "bnd_filename" );
	// if ( node_bnd )
	// { bnd_filename = PhysiCell::xml_get_my_string_value (node_bnd); }
	
	// pugi::xml_node node_cfg = node.child( "cfg_filename" );
	// if ( node_cfg )
	// { cfg_filename = PhysiCell::xml_get_my_string_value (node_cfg); }
	
	// pugi::xml_node node_init_values = node.child( "initial_values" );
	// if( node_init_values )
	// {
	// 	pugi::xml_node node_init_value = node_init_values.child( "initial_value" );
	// 	while( node_init_value )
	// 	{
	// 		std::string node_name = node_init_value.attribute( "node" ).value(); 
	// 		double node_value = PhysiCell::xml_get_my_double_value( node_init_value );
			
	// 		initial_values[node_name] = node_value;
			
	// 		node_init_value = node_init_value.next_sibling( "initial_value" ); 
	// 	}
	// }
	
	// pugi::xml_node node_mutations = node.child( "mutations" );
	// if( node_mutations )
	// {
	// 	pugi::xml_node node_mutation = node_mutations.child( "mutation" );
	// 	while( node_mutation )
	// 	{
	// 		std::string node_name = node_mutation.attribute( "node" ).value(); 
	// 		double node_value = PhysiCell::xml_get_my_double_value( node_mutation );
			
	// 		mutations[node_name] = node_value;
			
	// 		node_mutation = node_mutation.next_sibling( "mutation" ); 
	// 	}
	// }

	pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        sbml_filename = PhysiCell::xml_get_my_string_value (node_sbml); 
        std::cout << __FILE__ << ", " << __FUNCTION__ << ": ------- sbml_filename = " << sbml_filename << std::endl;
    }
	
	pugi::xml_node node_parameters = node.child( "parameters" );
	if( node_parameters )
	{
		pugi::xml_node node_parameter = node_parameters.child( "parameter" );
		while( node_parameter )
		{
			std::string param_name = node_parameter.attribute( "name" ).value(); 
			double param_value = PhysiCell::xml_get_my_double_value( node_parameter );
			
			parameters[param_name] = param_value;
			
			node_parameter = node_parameter.next_sibling( "parameter" ); 
		}
	}
	
	// maboss.init_maboss(bnd_filename, cfg_filename);
	// maboss.mutate(mutations);
	// maboss.set_initial_values(initial_values);
	// maboss.set_parameters(parameters);	
	
	// pugi::xml_node node_timestep = node.child( "time_step" ); 
	// if( node_timestep )
	// { 
	// 	time_step = PhysiCell::xml_get_my_double_value( node_timestep );
	// 	maboss.set_update_time_step(time_step);
	// }
}

SBMLIntracellular* getSBMLModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<SBMLIntracellular*>(phenotype.intracellular);
}

// void SBMLIntracellular::save_PhysiBoSS(std::string path, std::string index)
// {

// 	std::string state_file_name = path + "/states_" + index + ".csv";
					
// 	std::ofstream state_file( state_file_name );
	
// 	state_file << "ID,state" << std::endl;

// 	for( auto cell : *PhysiCell::all_cells )
// 		state_file << cell->ID << "," << cell->phenotype.intracellular->get_state() << std::endl;
		
// 	state_file.close();

// }