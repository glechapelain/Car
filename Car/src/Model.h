#pragma once
#include <string>

struct Model;

class Graphics {
public:
	Graphics(std::string const& filename, std::string const& texturename);
	Graphics(Graphics const&);

	void Render(Vector3 const &position, float angle) const;
	virtual ~Graphics();

private:

	struct Resources {
		Model model;
		Texture2D texture;
	} mResources;

	void Load();

	std::string const& mFileName;
	std::string const& mTextureName;
};
