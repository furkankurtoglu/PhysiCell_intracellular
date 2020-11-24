#include "librr_intracellular.h"

#include <sstream>

RoadRunnerIntracellular::RoadRunnerIntracellular() : Intracellular()
{
	type = "sbml";
    std::cout << "====== " << __FUNCTION__ << "() type=" << type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "() sbml_file = " <<  sbml_file << std::endl;
	// initial_values.clear();
	// mutations.clear();
	parameters.clear();
}

// constructor using XML node
RoadRunnerIntracellular::RoadRunnerIntracellular(pugi::xml_node& node)
{
    std::cout << "====== " << __FUNCTION__ << ": node.name() =" << node.name() << std::endl;
	type = "roadrunner";
	initialize_intracellular_from_pugixml(node);
    std::cout << "====== " << __FUNCTION__ << "(node) type=" << type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "(node) sbml_file = " <<  sbml_file << std::endl;
    std::cout << "====== " << __FUNCTION__ << "(node) this=" <<  this << std::endl;
}

RoadRunnerIntracellular::RoadRunnerIntracellular(RoadRunnerIntracellular* copy) 
{
	type = copy->type;
	// cfg_filename = copy->cfg_filename;
	// time_step = copy->time_step;
	// discrete_time = copy->discrete_time;
	// time_tick = copy->time_tick;
	// scaling = copy->scaling;
	// initial_values = copy->initial_values;
	// mutations = copy->mutations;
	parameters = copy->parameters;
	
}

// Parse the <intracellular> info in the .xml for (possibly) each <cell_definition ...>, e.g.
// <intracellular type="roadrunner">
// 	<sbml_filename>./config/Toy_SBML_Model_2.xml</sbml_filename>
// 	<time_step>1</time_step>
//     <species substrate="oxygen">Oxy</species>
//     <species substrate="glucose">Glc</species>
//     <species custom_data="energy">Energy</species>
void RoadRunnerIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        this->sbml_file = PhysiCell::xml_get_my_string_value (node_sbml); 
        // sbml_file = PhysiCell::xml_get_my_string_value (node_sbml); 
        // std::cout << "\n========== " << __FILE__ << ", " << __FUNCTION__ << ": ------- sbml_file = " << sbml_file << std::endl;
        // std::cout << "\n------------- "  << __FUNCTION__ << ": sbml_file = " << sbml_file << std::endl;
        std::cout << "\n------------- "  << __FUNCTION__ << ": this->sbml_file = " << this->sbml_file << std::endl;
    }
	
	pugi::xml_node node_species = node.child( "species" );
	while( node_species )
	{
        // ---------  substrate
		std::string substrate_name = node_species.attribute( "substrate" ).value(); 
		if( substrate_name != "" )
		{
			std::string species_name = PhysiCell::xml_get_my_string_value( node_species );
			this->substrate_species[substrate_name] = species_name;
            // std::cout << "\n------------- "  << __FUNCTION__ << ": species_name= " << species_name << std::endl;
		}

        // ---------  custom_data
		std::string custom_data_name = node_species.attribute( "custom_data" ).value(); 
		if( custom_data_name != "" )
		{
			std::string species_name = PhysiCell::xml_get_my_string_value( node_species );
			this->custom_data_species[custom_data_name] = species_name;
            // std::cout << "\n------------- "  << __FUNCTION__ << ": species_name= " << species_name << std::endl;
		}

		node_species = node_species.next_sibling( "species" ); 
	}
	
    std::cout << "  ------- substrate_species map:"  << std::endl;
    for(auto elm : this->substrate_species)
    {
        std::cout << "      "  << elm.first << " -> " << elm.second << std::endl;
    }
    std::cout << "  ------- custom_data_species map:"  << std::endl;
    for(auto elm : custom_data_species)
    {
        std::cout << "      "  << elm.first << " -> " << elm.second << std::endl;
    }
    std::cout << std::endl;


	// maboss.set_initial_values(initial_values);
	// maboss.set_parameters(parameters);	
	// pugi::xml_node node_timestep = node.child( "time_step" ); 
	// if( node_timestep )
	// { 
	// 	time_step = PhysiCell::xml_get_my_double_value( node_timestep );
	// 	maboss.set_update_time_step(time_step);
	// }
}

int RoadRunnerIntracellular::start()
{
    rrc::RRVectorPtr vptr;

    std::cout << "\n------------ " << __FUNCTION__ << "------ librr_intracellular.cpp: start() called\n";
    this->rrHandle = createRRInstance();

    // if (!rrc::loadSBML (rrHandle, get_cell_definition("lung epithelium").sbml_filename.c_str())) 
    std::cout << "     sbml_file = " << sbml_file << std::endl;

    // TODO: don't hard-code name
    // if ( !rrc::loadSBML(rrHandle, (this->sbml_file).c_str() ) )
    if (!rrc::loadSBML(rrHandle, "./config/Toy_SBML_Model_1.xml") )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        return -1;
        // 	printf ("Error message: %s\n", getLastError());
        // 	exit (0);
    }

    // std::cout << "     rrHandle=" << rrHandle << std::endl;

    int r = rrc::getNumberOfReactions(rrHandle);
    int m = rrc::getNumberOfFloatingSpecies(rrHandle);
    int b = rrc::getNumberOfBoundarySpecies(rrHandle);
    int p = rrc::getNumberOfGlobalParameters(rrHandle);
    int c = rrc::getNumberOfCompartments(rrHandle);

    std::cerr << "Number of reactions = " << r << std::endl;
    std::cerr << "Number of floating species = " << m << std::endl;  // 4
    std::cerr << "Number of boundary species = " << b << std::endl;  // 0
    std::cerr << "Number of compartments = " << c << std::endl;  // 1

    std::cerr << "Floating species names:\n";
    std::cerr << "-----------------------\n";
    std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(rrHandle));
    std::cerr <<  species_names_str <<"\n"<< std::endl;
    std::stringstream iss(species_names_str);
    std::string species_name;
    int idx = 0;
    while (iss >> species_name)
    {
        species_result_column_index[species_name] = idx;
        std::cout << species_name << " -> " << idx << std::endl;
        idx++;
    }

    vptr = rrc::getFloatingSpeciesConcentrations(rrHandle);
    std::cerr << vptr->Count << std::endl;
    for (int kdx=0; kdx<vptr->Count; kdx++)
    {
        std::cerr << kdx << ") " << vptr->Data[kdx] << std::endl;
    }
    std::cerr << "----------  end start() -------------\n";

    return 0;
}

bool RoadRunnerIntracellular::need_update()
{
    return PhysiCell::PhysiCell_globals.current_time >= this->next_librr_run;
}

int RoadRunnerIntracellular::update()
{
    static double start_time = 0.0;
    static double end_time = 0.01;
    static int num_vals = 1;

    // result = rrc::simulateEx (pCell->phenotype.molecular.model_rr, 0, 10, 10);  // start time, end time, and number of points
    // std::cout << "----- update(): rrHandle=" << this->rrHandle << std::endl;
    result = rrc::simulateEx (this->rrHandle, start_time, end_time, num_vals);  // start time, end time, and number of points

    // this->next_librr_run += this->rrHandle.get_time_to_update();
    // std::cout << "----- update(): result=" << result << std::endl;
    std::cout << "----- update(): result->CSize=" << result->CSize << std::endl;
    std::cout << "----- update(): result->RSize=" << result->RSize << std::endl;  // should be = num_vals
    // std::cout << "----- update(): result->ColumnHeaders[0]=" << result->ColumnHeaders[0] << std::endl;  // = "time"

    // debug - does it generate expected data?
    int index = 0;
    // Print out column headers... typically time and species.
    for (int col = 0; col < result->CSize; col++)
    {
        // std::cout << result->ColumnHeaders[index++];
        // std::cout << std::left << std::setw(15) << result->ColumnHeaders[index++];
        std::cout << std::left << result->ColumnHeaders[index++];
        // if (col < result->CSize - 1)
        // {
        // 	// std::cout << "\t";
        // 	std::cout << "  ";
        // }
    }
    std::cout << "\n";

    index = 0;
    // Print out the data
    for (int row = 0; row < result->RSize; row++)
    {
        for (int col = 0; col < result->CSize; col++)
        {
            // std::cout << result->Data[index++];
            std::cout << std::left << std::setw(15) << result->Data[index++];
            // if (col < result->CSize -1)
            // {
            // 	// std::cout << "\t";
            // 	std::cout << "  ";
            // }
        }
        std::cout << "\n";
    }
    // int idx = (result->RSize - 1) * result->CSize + 1;
    // std::cout << "Saving last energy value (cell custom var) = " << result->Data[idx] << std::endl;
    // pCell->custom_data[energy_cell_idx]  = result->Data[idx];

    return 0;
}

double RoadRunnerIntracellular::get_parameter_value(std::string substrate_name)
{
    std::cout << "-----------"  << __FUNCTION__ << "-----------" << std::endl;
    std::cout << "    substrate_name = " << substrate_name << std::endl;

    // TODO: optimize this eventually
    // std::map<std::string, int> species_result_column_index;
    // int num_columns = result->CSize;
    // int offset = (num_rows_result_table-1) * result->CSize - 1;
    // int offset = (num_rows_result_table-1) * result->CSize;
    // offset += (num_rows_result_table-1) * result->CSize - 1;
    // int offset = species_result_column_index[name];
    std::string species_name = this->substrate_species[substrate_name];
    std::cout << "    species_name = " << species_name << std::endl;

    int offset = species_result_column_index[species_name];
    std::cout << "    result offset = "<< offset << std::endl;
    double res = this->result->Data[offset];
    std::cout << "    res = " << res << std::endl;
	return res;
}
	
int RoadRunnerIntracellular::set_parameter_value(std::string name, double value)
{
    return 0;
}

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<RoadRunnerIntracellular*>(phenotype.intracellular);
}

// void RoadRunnerIntracellular::save_PhysiBoSS(std::string path, std::string index)
// {
// 	std::string state_file_name = path + "/states_" + index + ".csv";
// 	std::ofstream state_file( state_file_name );
// 	state_file << "ID,state" << std::endl;
// 	for( auto cell : *PhysiCell::all_cells )
// 		state_file << cell->ID << "," << cell->phenotype.intracellular->get_state() << std::endl;
// 	state_file.close();
// }

std::string RoadRunnerIntracellular::get_state()
{
    return "bogus_libroadrunner_sbml_state";
}