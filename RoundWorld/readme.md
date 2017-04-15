1. Juliana Seng

2. What you are drawing
- buger system revolving and expanding around the meat patty, and 20 pieces nuggets. the background is coke btw

3. Primitives and transformations you have used
- GL_POLYGON
- GL_QUADS
- glTranslatef
- glRotatef
- glScalef

4. Methods you have modified
- Added methods: drawEllipse, drawSesemeSeed, drawTopBun, drawTomato, drawPatty, drawCheese, drawBottomBun, drawVeggie, getAngle, drawBurger, drawNuggetBack, drawNuggetFront, drawCokeBackground, addNugget, initStar
- Added classes: nugget, star
- Changed parameters of glOrtho() in reshape() to give us a bigger canvas
- Changed parameters of glClearColor() in init() to give us coke background
- Added keypress case "n" in keyboard() to add new nugget
- Added cout instruction in main() for "n" keypress
- Added initStar() in main() to do one time initialization of stars

5. Any other things the TA should know?
- Here are the resources I have referenced
- Burger: https://freeiconshop.com/icon/burger-icon-flat/

6. Bonus
- Stars that fade and glow
- Unlimited number of nuggets orbit elliptically, drawn underneath burger when it goes behind
- Burger ingredient drift further apart from each other cause the universe is expanding?