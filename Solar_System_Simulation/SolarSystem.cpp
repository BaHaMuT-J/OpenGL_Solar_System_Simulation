#include <header/camera.h>
#include <header/solar.h>

int main() {
	bool scaledSize = true;
	bool backgroundBlack = false;

	// comment next line if you want scaled size version
	//scaledSize = false;

	// comment next line if you want black background instead of grey
	//backgroundBlack = true;

	if (scaledSize) 
	{
		camera = Camera(glm::vec3(0.0f, 0.0f, 80.0f));
		return solarScaledSize(backgroundBlack);
	}
	else 
	{
		camera = Camera(glm::vec3(0.0f, 0.0f, 20.0f));
		return solarScaledDistance(backgroundBlack);
	}
}