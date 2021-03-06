%module crate_game_engine
%{
#include "game_personality/script.hpp"
#include "game_personality/world_change_set.hpp"
#include "game_personality/world_script_access.hpp"
%}

%include "stl.i"

%include "../mvc/object_id.hpp"

%template(vector_object_id) std::vector<mvc::object_id_base>;

%include "objects.hpp"

%template(object_id_game_object) mvc::object_id<game_personality::game_object>;

%include "script.hpp"

%include "world_writer.hpp"
%include "world_reader.hpp"


%include "world_change_set.hpp"
%include "world_script_access.hpp"


%include "events.hpp"

