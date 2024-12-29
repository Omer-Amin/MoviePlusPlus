#include "headers.h"

// should be able to pass a parameter like "FROM_LAST_SCENE" to easy continue on states and not have to create duplicates

int main() 
{
    // Create an SDL2 renderer and window with dimension 500x500
    Camera camera(400, 225);
    // Create an animation handler, set FPS to 60
    Movie movie(camera, 60);

    // Create an actor with a square appearance
    Actor square({ {100, 100}, {200, 100}, {200, 200}, {100, 200} });
    square.setPosition({ camera.WIDTH / 2.0f, camera.HEIGHT / 2.0f });

    // Create an actor with a triangle appearance and move to square's position
    Actor tri({ {300, 250}, {350, 350}, {250, 350} });
    tri.setPosition(square.position);

    // Create an actor with some arbitrary shape (sketch)
    Actor sketch({
        {122, 202}, {122, 202}, {116, 202}, {107, 199},
        {99, 189},  {102, 187}, {96, 179},  {85, 166},
        {88, 165},  {84, 158},  {83, 151},  {91, 148},
        {95, 130},  {97, 120},  {106, 117}, {114, 92},
        {126, 85},  {135, 78},  {139, 78},  {144, 83},
        {155, 88},  {176, 93},  {179, 93},  {178, 106},
        {180, 111}, {178, 116}, {192, 116}, {201, 117},
        {206, 123}, {210, 137}, {212, 146}, {214, 160},
        {181, 169}, {164, 174}, {170, 195}, {173, 208},
        {138, 225}, {133, 215}, {136, 207}, {131, 206},
        {127, 204}, {126, 201}
    });
    sketch.setPosition(square.position);

    // Create scene for half second delay
    Scene delay = Writer::wait(0.5);

	// Create a scene to draw the sketch
    Scene drawSketch = Writer::draw(Algebra::close(sketch.vertices), 2, Easing::InOutQuad);
 
    // Create a generic action to grow an actor from 0 to 1 with elastic easing in 1.5s
    Action grow(M_SCALE, {Action::arg(0, 0), Action::arg(1, 1)}, Action::arg(0, 0), 1.5, Easing::OutElastic);
    // Append the action to the sketch actor
    Director::appendAction(sketch, grow);
    // Create a scene of the sketch actor doing the grow action
    Scene growSketch(sketch);

    // Create scene to morph the sketch into a square
    Scene morphSketchToSquare = Writer::morph(sketch.vertices, square.vertices, 2, Easing::InOutElastic);

    // Create a scene to morph the square into a triangle
    Scene morphSquareToTriangle = Writer::morph(square.vertices, tri.vertices, 2, Easing::InOutElastic);

    // Create a generic action to rotate an actor about the square's position by 2 pi radians
    Action rotate(M_ROTATE, {Action::arg(0), Action::arg(Easing::PI * 2)},
        [&](Actor& actor){
            return square.position - actor.position;
        },
        2, Easing::InOutElastic);

    // Get a path modifier to modify the morphSquareToTriangle scene according to the rotate action
    Modifier angleModifier = Director::getScenePathModifier(morphSquareToTriangle, rotate);

    // Apply the modifier while the animation is running
    morphSquareToTriangle.afterTick = [&](Scene& scene) {
        angleModifier.apply(scene, angleModifier);
    };

    // Create a generic action to shrink an actor by reversing the grow action
    Action shrink(grow.reverse());
    // Give the shrink action a different easing function
    shrink.easing = Easing::InOutExpo;
    // Append the action to the tri actor
    Director::appendAction(tri, shrink);
    // Create a scene of the tri actor doing the shrink action
    Scene shrinkTriangle(tri);

    // Dictate the order in which the scenes play
    movie.compose({{delay}, {drawSketch}, {morphSketchToSquare}, {morphSquareToTriangle}, {shrinkTriangle}});

    // Loop the animation forever
    movie.setCycles(M_FOREVER);

    // Run the animation in an SDL2 window
    movie.play();

    return 0;
}
