#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <string>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	std::vector<uint16_t> generate_randint_arr(int size, uint16_t upperBound = 11);

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	//hexapod leg to wobble:
	/*Scene::Transform *hip = nullptr;
	Scene::Transform *upper_leg = nullptr;
	Scene::Transform *lower_leg = nullptr;
	glm::quat hip_base_rotation;
	glm::quat upper_leg_base_rotation;
	glm::quat lower_leg_base_rotation;*/
	float wobble = 0.0f;

	//glm::vec3 get_leg_tip_position();

	// level 1 - 3 pairs, 2 - 4 pairs...
	uint8_t level;
	const uint8_t LEVEL[3] = { 3, 4, 5 };

	struct Card {
		Scene::Drawable* drawable;
		uint16_t soundIndex;
		Sound::Sample* sound;
		glm::vec2 location;
		float flip_count = 0.0f;	// 0 means normal, while 90 means back
		bool locked = false;		// lock if player got it
	};

	Card* card;

	uint8_t locked_pairs = 0;

	const std::string sound_file_string[11] = { "Random 6.opus", "Random 7.opus" ,"Random 8.opus" , "Random 9.opus" , "Random 21.opus" , "Random 42.opus" , "Random 50.opus" ,
	"Random 54.opus" , "Random 56.opus" , "Random 71.opus" , "Random 87.opus" };

	//points to clicked and opened card
	int8_t clicked = -1, opened = -1;

	void place_cards(uint8_t pairs);

	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > leg_tip_loop;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
