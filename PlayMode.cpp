#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <algorithm>
#include <string>

#define WIDTH 0.7f
#define HEIGHT 1.0f

GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("card_obvious.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > hexapod_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("empty.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
	});
});

Load< Sound::Sample > dusty_floor_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Random 99(1).opus"));
});

void PlayMode::place_cards(uint8_t pairs) {
	//Mesh dummy;
	//Mesh& mesh = dummy;

	Mesh mesh = hexapod_meshes->lookup("Cube");
	
	float vertical_offset = 0.8f;
	float horizontal_space = (10.0f - pairs * WIDTH) / ((float)pairs + 2.0f);
	float hor_pos = -(horizontal_space * pairs / 2.0f);
	uint8_t i = 0;

	std::vector<uint16_t> rand_arr = generate_randint_arr(pairs);
	for (; i < pairs; i++) {
		auto newTrans = new Scene::Transform();
		scene.drawables.emplace_back(newTrans);
		Scene::Drawable& drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;
		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;

		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

		drawable.transform->position = glm::vec3(0.0f, hor_pos + i * horizontal_space, vertical_offset);

		auto sound = new Sound::Sample(data_path(sound_file_string[rand_arr.at(i)]));

		card[i].drawable = &drawable;
		card[i].sound = sound;
		card[i].soundIndex = rand_arr.at(i);
		card[i].location = glm::vec2(hor_pos + i * horizontal_space, vertical_offset);

	}
	vertical_offset = 0 - vertical_offset;
	hor_pos = -(horizontal_space * pairs / 2.0f);
	uint8_t j = 0;
	for (; i < pairs * 2; i++) {
		auto newTrans = new Scene::Transform();
		scene.drawables.emplace_back(newTrans);
		Scene::Drawable& drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;
		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;

		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

		drawable.transform->position = glm::vec3(0.0f, hor_pos + (j++)*horizontal_space, vertical_offset);

		auto sound = new Sound::Sample(data_path(sound_file_string[rand_arr.at(i)]));

		card[i].drawable = &drawable;
		card[i].sound = sound;
		card[i].soundIndex = rand_arr.at(i);
		card[i].location = glm::vec2(hor_pos + i * horizontal_space, vertical_offset);
	}
}

PlayMode::PlayMode() : scene(*hexapod_scene) {
	level = 2;
	card = new Card[(LEVEL[level - 1] * 2)];
	place_cards(LEVEL[level-1]);
	
	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
	camera->transform->position = glm::vec3(8.0f, 0.0f, 0.0f);

	//start music loop playing:
	// (note: position will be over-ridden in update())
	leg_tip_loop = Sound::loop(*dusty_floor_sample, 0.5f, 0.0f);
}

PlayMode::~PlayMode() {
	delete[] card;
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	glm::vec2 motion = glm::vec2(0.0f, 0.0f);
	if (evt.type == SDL_KEYDOWN) {
		//if (evt.key.keysym.sym == SDLK_ESCAPE) {
		//	//SDL_SetRelativeMouseMode(SDL_FALSE);
		//	return true;
		//}
		//else 
		if (evt.key.keysym.sym == SDLK_0) {
			clicked = 0;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_1) {
			clicked = 1;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_2) {
			clicked = 2;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_3) {
			clicked = 3;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_4) {
			clicked = 4;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_5) {
			clicked = 5;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_6) {
			clicked = 6;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_7) {
			clicked = 7;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_8) {
			clicked = 8;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_9) {
			clicked = 9;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_a) {
			clicked = 'a';
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_b) {
			clicked = 'b';
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_c) {
			clicked = 'c';
			return true;
		}


	}; /*else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	} else */
	//if (evt.type == SDL_MOUSEBUTTONDOWN) {
	//	//if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
	//	//	//SDL_SetRelativeMouseMode(SDL_TRUE);
	//	//	return true;
	//	//}
	//	int i = (evt.button.y > (float(window_size.y) / 2.0f)) ? LEVEL[level - 1] : 0;
	//	int max = (evt.button.y > (float(window_size.y) / 2.0f)) ? (LEVEL[level - 1] * 2) : LEVEL[level - 1];
	//	for (; i < max; i++) {
	//		if (card[i].drawable->transform->position.y - 0.4 < evt.button.x && card[i].drawable->transform->position.y + 0.4 > evt.button.x) {
	//			if (card[i].drawable->transform->position.z - 0.5 < evt.button.y && card[i].drawable->transform->position.z + 0.5 > evt.button.y) {
	//				
	//				std::cout << i << std::endl;
	//			}
	//		}
	//		
	//	}
	//	std::cout << card[3].drawable->transform->position.y - 0.4 << std::endl;
	//	std::cout << card[3].drawable->transform->position.y + 0.4 << std::endl;
	//	std::cout << i << std::endl;
	//	std::cout << evt.button.x - (float(window_size.y) / 2.0f) << " " << -(evt.button.y - (float(window_size.y) / 2.0f)) << std::endl;


	//	return true;
	//} else if (evt.type == SDL_MOUSEMOTION) {
	//	//if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
	//	//	motion = glm::vec2(
	//	//		evt.motion.xrel / float(window_size.y),
	//	//		-evt.motion.yrel / float(window_size.y)
	//	//	);
	//	//	/*camera->transform->rotation = glm::normalize(
	//	//		camera->transform->rotation
	//	//		* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
	//	//		* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
	//	//	);*/
	//	//	return true;
	//	//}
	//	motion = glm::vec2(
	//		evt.motion.x,
	//		-evt.motion.y
	//	);
	//	/*camera->transform->rotation = glm::normalize(
	//		camera->transform->rotation
	//		* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
	//		* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
	//	);*/
	//	return true;
	//}

	return false;
}

//void flip_cards(float elapsed) {
//	for (int i = 0; i < pairs * 2; i++) {
//		if (!card[i].locked) {
//			if (card[i].flip_count != 0.0f) {
//				
//			}
//		}
//	}
//}



int8_t old_opened = -1;
bool flipping = false;
int8_t flip_card = -1;
int8_t flip_card2 = -1;
float angle = 0.0f;
void PlayMode::update(float elapsed) {


	if (locked_pairs == LEVEL[level - 1]) {
		return;
	}

	if (flip_card != -1) {
		
		if (flipping == true && angle < 180.0f) {
			angle += 5.0f;
			if (card[flip_card].locked) {
				flip_card = -1;
				clicked = -1;
				assert(false);
				return;
			}
			card[flip_card].drawable->transform->rotation *= glm::angleAxis(
				glm::radians(5.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
			if (angle >= 180.0f) {
				flip_card = -1;
			}
		}
		else if (flipping == false && angle > 0) {
			angle -= 5.0f;
			if (card[flip_card].locked) {
				flip_card = -1;
				clicked = -1;
				assert(false);
				return;
			}
			if (card[flip_card2].locked) {
				flip_card2 = -1;
				clicked = -1;
				assert(false);
				return;
			}
			card[flip_card].drawable->transform->rotation *= glm::angleAxis(
				glm::radians(-5.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
			if (flip_card2 != -1) {
				card[flip_card].drawable->transform->rotation *= glm::angleAxis(
					glm::radians(-5.0f),
					glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (angle <= 0.0f) {
				flip_card = -1;
				flip_card2 = -1;
			}
		}

		//clicked = -1;
		return;
	}

	if (opened == -1 && clicked != -1) {	//player clicked something, and nothing opened
		opened = clicked;
		old_opened = clicked;
		Sound::play(*card[opened].sound);
		flip_card = opened;
		flipping = true;
		
	}
	else if (clicked != -1)
	{
		if (clicked != opened) {	// user press another key
			flip_card = clicked;
			flipping = true;
			Sound::play(*card[clicked].sound);
			if (card[clicked].soundIndex == card[opened].soundIndex) {
				card[clicked].locked = true;
				card[opened].locked = true;
				locked_pairs++;
			}
			else {
				opened = -1;
				flip_card = clicked;
				flip_card2 = opened;
				flipping = false;
			}
		}
	}
	clicked = -1;

	/*if (old_opened != opened) {
		Sound::play(card[opened].sound);
		old_opened = opened;
	}*/

	//slowly rotates through [0,1):
	/*wobble += elapsed / 10.0f;
	wobble -= std::floor(wobble);*/

	/*hip->rotation = hip_base_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	upper_leg->rotation = upper_leg_base_rotation * glm::angleAxis(
		glm::radians(7.0f * std::sin(wobble * 2.0f * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	lower_leg->rotation = lower_leg_base_rotation * glm::angleAxis(
		glm::radians(10.0f * std::sin(wobble * 3.0f * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);*/

	//move sound to follow leg tip position:
	//leg_tip_loop->set_position(get_leg_tip_position(), 1.0f / 60.0f);

	//move camera:
	{

		//combine inputs into a move:
		//constexpr float PlayerSpeed = 30.0f;
		//glm::vec2 move = glm::vec2(0.0f);
		//if (left.pressed && !right.pressed) move.x =-1.0f;
		//if (!left.pressed && right.pressed) move.x = 1.0f;
		//if (down.pressed && !up.pressed) move.y =-1.0f;
		//if (!down.pressed && up.pressed) move.y = 1.0f;

		////make it so that moving diagonally doesn't go faster:
		//if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		//glm::mat4x3 frame = camera->transform->make_local_to_parent();
		//glm::vec3 frame_right = frame[0];
		////glm::vec3 up = frame[1];
		//glm::vec3 frame_forward = -frame[2];

		//camera->transform->position += move.x * frame_right + move.y * frame_forward;
	}

	{ //update listener to camera position:
		/*glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		glm::vec3 frame_at = frame[3];
		Sound::listener.set_position_right(frame_at, frame_right, 1.0f / 60.0f);*/
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

std::string int_to_string(int a) {
	switch (a)
	{
	case 12:
		return "c";
	case 11:
		return "b";
	case 10:
		return "a";
	case -1:
		return "None";
	default:
		//std::string m = ;
		return std::to_string(a);
		break;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("Click the pair of card of the same sound: First ROW: [0-3], Second ROW: [4-7]",
			glm::vec3(-aspect + 0.1f * H, /*-1.0 + 0.1f * H, 0.0*/ 0.90, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Opened: " + int_to_string(opened),
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.80, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		lines.draw_text("Matched Pairs: " + int_to_string(locked_pairs),
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.70, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		lines.draw_text("Level: " + std::to_string(level),
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));

	}
	GL_ERRORS();
}

//glm::vec3 PlayMode::get_leg_tip_position() {
//	//the vertex position here was read from the model in blender:
//	return lower_leg->make_local_to_world() * glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f);
//}

std::vector<uint16_t> PlayMode::generate_randint_arr(uint16_t size) {
	std::vector<uint16_t> arr(size * 2);

	// fill the array with integers from 0 to n-1
	for (uint16_t i = 0; i < size; ++i) {
		arr[i] = i;
		arr[i + 1] = i;
	}

	// random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// shuffle the array
	std::shuffle(arr.begin(), arr.end(), gen);

	return arr;
}

//node Maekfile.js && cd dist && game.exe && cd ..