#include <header/camera.h>
#include <header/solar.h>

int main() {
	bool scaledSize = true;

	// comment next line if you want scaled size version
	//scaledSize = false;

	if (scaledSize) 
	{
		camera = Camera(glm::vec3(0.0f, 0.0f, 80.0f));
		return solarScaledSize();
	}
	else 
	{
		camera = Camera(glm::vec3(0.0f, 0.0f, 20.0f));
		return solarScaledDistance();
	}
}