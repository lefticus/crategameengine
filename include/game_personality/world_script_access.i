%module crate_game_engine
%{
#include "objects.hpp"
#include "world_reader.hpp"
#include "personality_interfaces.hpp"

%}

%include "stl.i"

%include "../mvc/object_id.hpp"

%template(vector_object_id) std::vector<mvc::object_id_base>;

%include "objects.hpp"

%template(object_id_game_object) mvc::object_id<game_personality::game_object>;

%include "../mvc/script.hpp"
%include "world_reader.hpp"
%include "world_writer.hpp"
%include "world_change_set.hpp"

%template(world_script_access_game) mvc::world_script_access<game_personality::world_change_set>;

%include "world_script_access.hpp"

%include "events.hpp"

