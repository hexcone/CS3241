1. Juliana Seng

2. What you are drawing
- A toucan

3. Primitives and transformations you have used
- GL_QUAD_STRIP
- GL_TRIANGLE_FAN
- GL_TRIANGLES
- glTranslatef

4. Methods you have modified
- Added 3 methods: drawBackground(), drawSun and drawTriangle()
- Changed parameters of glOrtho() in reshape() to give us a bigger canvas
- Added glEnable() and glBlendFunc() in main() for transparency

5. Any other things the TA should know?
- Here are the resources I have referenced
- Photograph of a toucan: http://www.wallpapers13.com/toucan-bird-family-ramphastidae-of-near-passerine-birds-from-the-neotropics-wallpaper-hd-3840x2400/
- Image triangulator: http://somestuff.ru/I into vectors.

6. Bonus
- Background uses GL_QUAD_STRIP to form several vertical points and blends in shades of green to create a gradient
- Sun uses GL_TRIANGLE_FAN and alpha values to make edges transparent
- Alot of effort in drawing many triangles lol