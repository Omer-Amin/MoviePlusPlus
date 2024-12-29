


// std::vector<Vec2> points;


    // movie.events = [&](SDL_Event& e) {
    //     int mouseX, mouseY;
    //     Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    //     if (e.type == SDL_KEYDOWN) {
    //         if (e.key.keysym.sym == SDLK_d && mouseState) {
    //             points.push_back(Vec2((float)mouseX, (float)mouseY));
    //             std::cout << "{" << (float)mouseX << ", " << (float)mouseY << "}," << std::endl;
    //         }
    //     }
    // };

    // movie.renderMethod = [&](Movie& movie) {
    //     camera.clear(M_BLACK);
    //     for (auto& scene : movie.scenes[movie.activeScene]) {
    //         scene.renderMethod(scene, movie.camera);
    //     }
    //     if (points.size() > 1) camera.poly(points, false);
    //     camera.render();
    // };