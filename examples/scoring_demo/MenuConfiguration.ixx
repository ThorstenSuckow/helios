/**
 * @file MenuConfiguration.ixx
 * @brief Menu and UI element configuration for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.MenuConfiguration;

import helios;
import helios.ext;
import helios.examples.scoring.IdConfig;
import helios.examples.scoring.CollisionId;

export namespace helios::examples::scoring {

    /**
     * @brief Creates all menu, title, HUD, and game-over UI elements.
     *
     * @param gameWorld The game world to create UI GameObjects in.
     * @param renderingDevice The rendering device for font loading.
     * @param glyphShader The shared text shader.
     * @param defaultShader The shared mesh shader.
     * @param titleScene Scene for title screen elements.
     * @param menuScene Scene for pause menu elements.
     * @param hudScene Scene for HUD elements.
     */
    inline void configureMenus(
        helios::engine::runtime::world::GameWorld& gameWorld,
        helios::rendering::RenderingDevice& renderingDevice,
        std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> glyphShader,
        std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> defaultShader,
        helios::scene::Scene& titleScene,
        helios::scene::Scene& menuScene,
        helios::scene::Scene& hudScene
    ) {
        using namespace helios::engine::runtime::world;
        using namespace helios::engine::builder::gameObject;
        using namespace helios::engine::ecs;
        using namespace helios::ext::opengl::rendering::shader;
        using namespace helios::ext::opengl::rendering;
        using namespace helios::ext::opengl;
        using namespace helios::scene;
        using namespace helios::rendering;


            auto* level = gameWorld.level();

            auto gameTitleFont = helios::engine::core::data::FontId{"titleBig"};
            auto& fontResourceProvider = renderingDevice.fontResourceProvider();
            fontResourceProvider.loadFont(gameTitleFont, 96, "resources/Tiny5/Tiny5-Regular.ttf");//"Roboto-SemiBoldItalic.ttf");

            auto uiTextFont = helios::engine::core::data::FontId{"uiMed"};
            fontResourceProvider.loadFont(uiTextFont, 32, "resources/Tiny5/Tiny5-Regular.ttf");

            // ========================================================================
            // 5.6. Text Prototype and Renderable Creation
            // ========================================================================
            // Create a reusable text prototype with shader and font configuration.
            auto uiTextPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
                glyphShader,
                std::make_shared<helios::rendering::text::TextShaderProperties>(),
                &fontResourceProvider
            );


            auto titleText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("helios ascending")
                       .fontId(gameTitleFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::WhiteSmoke)
                       .attachTo(&titleScene.root());

                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::Center)
                      .viewport(helios::engine::core::data::ViewportId{"titleViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({-100.0f, 0.0f, 0.0f, 0.0f});

                })
                .make();

            auto pressStartText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("press start")
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(&titleScene.root());

                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::Center)
                      .viewport(helios::engine::core::data::ViewportId{"titleViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({40.0f, 0.0f, 0.0f, 0.0f});

                })
                .make();


            auto menuBox = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::Turquoise.withW(0.8f))
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
                       .attachTo(&menuScene.root());
                })
                .withMenu([](auto& mb) {
                    mb.menu()
                      .menuId(helios::engine::core::data::MenuId{"MainMenu"});
                })
                .withTransform([](auto& tb) {
                    tb.transform()
                      .scale(helios::math::vec3f(600.0f, 400.0f, 0.0f));
                })
                 .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::Center)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({0.0f, 0.0f, 0.0f, 0.0f});
                })
                .make();


            auto continueText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("continue")
                       .asUiText()
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(menuBox)
                       .inherit(helios::math::TransformType::Translation);

                })

                .withMenu([&](auto& mb) {
                    mb.menuItem(menuBox)
                      .index(0)
                      .normalScale(1.0f)
                      .selectedScale(1.2f)
                      .actionId(helios::engine::core::data::ActionId{"continueGame"})
                      .normalColor(helios::util::Colors::LightGray)
                      .selectedColor(helios::util::Colors::White)
                      .selected(true);
                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .offsets({80.0f, 0.0f, 0.0f, 80.0f});

                })
                .make();

            auto restartText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("restart")
                       .asUiText()
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(menuBox)
                       .inherit(helios::math::TransformType::Translation);

                })
                .withMenu([&](auto& mb) {
                     mb.menuItem(menuBox)
                       .index(1)
                       .normalScale(1.0f)
                       .selectedScale(1.2f)
                       .actionId(helios::engine::core::data::ActionId{"restartGame"})
                       .normalColor(helios::util::Colors::LightGray)
                       .selectedColor(helios::util::Colors::White)
                       .selected(false);
                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .offsets({120.0f, 0.0f, 0.0f, 80.0f});

                })
                .make();

            auto quitText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("quit")
                       .asUiText()
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(menuBox)
                       .inherit(helios::math::TransformType::Translation);

                })
                .withMenu([&](auto& mb) {
                    mb.menuItem(menuBox)
                      .normalColor(helios::util::Colors::LightGray)
                      .normalScale(1.0f)
                      .selectedScale(1.2f)
                      .actionId(helios::engine::core::data::ActionId{"quitGame"})
                      .selectedColor(helios::util::Colors::White)
                      .index(2)
                      .selected(false);
                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .offsets({160.0f, 0.0f, 0.0f, 80.0f});

                })
                .make();

            auto gameOverMenu = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::HotPink.withW(0.8f))
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
                       .attachTo(&menuScene.root());
                })
                .withMenu([](auto& mb) {
                    mb.menu()
                      .menuId(helios::engine::core::data::MenuId{"GameOverMenu"});
                })
                .withTransform([](auto& tb) {
                    tb.transform()
                      .scale(helios::math::vec3f(600.0f, 400.0f, 0.0f));
                })
                 .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::Center)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({0.0f, 0.0f, 0.0f, 0.0f});
                })
                .make();


            auto gameOverText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("Game Over")
                       .asUiText()
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::WhiteSmoke)
                       .attachTo(gameOverMenu)
                       .inherit(helios::math::TransformType::Translation);

                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::Center)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({-40.0f, 0.0f, 0.0f, 0.0f});

                })
                .make();

            auto gameOverRetryText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                    .gameObject(gameWorld)
                    .withRendering([&](auto& rnb) {
                        rnb.textRenderable()
                           .text("retry")
                           .asUiText()
                           .fontId(uiTextFont)
                           .fontScale(1.0f)
                           .fontResourceProvider(renderingDevice.fontResourceProvider())
                           .shader(glyphShader)
                           .color(helios::util::Colors::LightGray)
                           .attachTo(gameOverMenu)
                           .inherit(helios::math::TransformType::Translation);

                    })
                    .withMenu([&](auto& mb) {
                        mb.menuItem(gameOverMenu)
                          .normalColor(helios::util::Colors::LightGray)
                          .normalScale(1.0f)
                          .selectedScale(1.2f)
                          .actionId(helios::engine::core::data::ActionId{"gameOverRetry"})
                          .selectedColor(helios::util::Colors::White)
                          .index(0)
                          .selected(true);
                    })
                    .withUiTransform([](auto& tb) {
                        tb.transform()
                          .pivot(helios::engine::modules::ui::layout::Anchor::TopLeft)
                          .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                          .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                          .offsets({20.0f, 0.0f, 0.0f, -40.0f});

                    })
                    .make();


            auto gameOverQuitText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("exit")
                       .asUiText()
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(gameOverMenu)
                       .inherit(helios::math::TransformType::Translation);

                })
                .withMenu([&](auto& mb) {
                    mb.menuItem(gameOverMenu)
                      .normalColor(helios::util::Colors::LightGray)
                      .normalScale(1.0f)
                      .selectedScale(1.2f)
                      .actionId(helios::engine::core::data::ActionId{"gameOverQuitGame"})
                      .selectedColor(helios::util::Colors::White)
                      .index(1)
                      .selected(false);
                })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopLeft)
                      .viewport(helios::engine::core::data::ViewportId{"menuViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::Center)
                      .offsets({60.0f, 0.0f, 0.0f, 0.0f});

                })
                .make();

            auto scoreText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("0")
                       .fontId(uiTextFont)
                       .fontScale(1.0f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::WhiteSmoke)
                       .asUiText("{:.0f}")
                       .attachTo(&hudScene.root());

                })
                .withObserver([&](auto& ob) {
                   ob.observe()
                     .runningScore(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});
               })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopRight)
                      .viewport(helios::engine::core::data::ViewportId{"hudViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::TopRight)
                      .offsets({60.0f, 50.0f, 0.0f, 0.0f});

                })
                .make();

            auto highScoreText = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withRendering([&](auto& rnb) {
                    rnb.textRenderable()
                       .text("Highscore: 0")
                       .asUiText("Highscore: {:.0f}")
                       .fontId(uiTextFont)
                       .fontScale(0.8f)
                       .fontResourceProvider(renderingDevice.fontResourceProvider())
                       .shader(glyphShader)
                       .color(helios::util::Colors::LightGray)
                       .attachTo(&hudScene.root());
                })
               .withObserver([&](auto& ob) {
                   ob.observe()
                      .maxScore(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});
               })
                .withUiTransform([](auto& tb) {
                    tb.transform()
                      .pivot(helios::engine::modules::ui::layout::Anchor::TopRight)
                      .viewport(helios::engine::core::data::ViewportId{"hudViewport"})
                      .anchor(helios::engine::modules::ui::layout::Anchor::TopRight)
                      .offsets({25.0f, 50.0f, 0.0f, 0.0f});
                })
                .make();

            // projectile game object
            auto projectilePrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withPrefabId(IdConfig::ProjectilePrefab)
                .withRendering([&defaultShader, &root = *level->rootNode()](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::Yellow)
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Ellipse>(0.5f, 0.2f, 8))
                       .attachTo(&root);
                })
                .withTransform([](auto& tb) {
                    tb.transform()
                      .scale(helios::math::vec3f(2.2f, 0.8f, 0.0f), helios::core::units::Unit::Meter);
                })
                .withCollision([](auto& cb) {
                    cb.collision()
                      .layerId(helios::examples::scoring::CollisionId::Projectile)
                      .useBoundingBox()
                      .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
                      .reportCollisions(true)
                      .solidCollisionMask(helios::examples::scoring::CollisionId::Enemy)
                      .onSolidCollision(
                          helios::examples::scoring::CollisionId::Enemy,
                          helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn |
                          helios::engine::modules::physics::collision::types::CollisionBehavior::PassEvent
                      )
                      .dealDamage(100.0f, helios::examples::scoring::CollisionId::Enemy);

                    cb.levelBoundsCollision()
                      .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn);
                })
                .withMotion([](auto& mcb) {
                    mcb.move2D()
                       .speed(80.0f)
                       .instantAcceleration(true);
                    mcb.steering()
                       .steeringSetsDirection(false)
                       .instantSteering(true);
                })
                .withSpawn([](auto& sb) {
                     sb.spawn()
                       .useSpawnProfile()
                       .trackEmitter();
                 })
                .make();


            scoreText.setActive(true);
            highScoreText.setActive(true);
            titleText.setActive(true);
            pressStartText.setActive(true);

    }


}