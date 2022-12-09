#include "./GameWindow.hpp"
#include "../GameManager.hpp"

void GameWindow::OnCreate()
{
    renderTexture = ObjectManager::GetInstance()->CreateObject<TextureObject>();

    cardTextures[CardType::STATE_0_1] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::AND_0] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::AND_1] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::OR_0] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::OR_1] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::XOR_0] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::XOR_1] = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    cardTextures[CardType::EMPTY] = ObjectManager::GetInstance()->CreateObject<TextureObject>();

    auto& cards = MGetComponent(GameController)->GetCards();

    renderTexture.lock()->LoadEmpty({
        int(scaleTo.x) * int(cards.size()),
        int(scaleTo.y * narrowness) * int(cards.size() * 2 - 1) + int(scaleTo.y * .30f)
    });

    cardTextures[CardType::STATE_0_1].lock()->LoadFromFile("res/img/themes/cards/state-0-1.png");
    cardTextures[CardType::AND_0].lock()->LoadFromFile("res/img/themes/cards/and_0.png");
    cardTextures[CardType::AND_1].lock()->LoadFromFile("res/img/themes/cards/and_1.png");
    cardTextures[CardType::OR_0].lock()->LoadFromFile("res/img/themes/cards/or_0.png");
    cardTextures[CardType::OR_1].lock()->LoadFromFile("res/img/themes/cards/or_1.png");
    cardTextures[CardType::XOR_0].lock()->LoadFromFile("res/img/themes/cards/xor_0.png");
    cardTextures[CardType::XOR_1].lock()->LoadFromFile("res/img/themes/cards/xor_1.png");
    cardTextures[CardType::EMPTY].lock()->LoadFromFile("res/img/themes/cards/bg.png");
}

void GameWindow::OnUpdate()
{
    auto gameController = MGetComponent(GameController);
    
    if (gameController->IsGameOver())
        return;
    
    auto& deckCards = gameController->GetDeck();

    for (size_t i = 0; i < deckCards.size(); i++)
    {
        auto card = deckCards[i];
        Texture *texture = &cardTextures[card].lock()->GetTexture();

        DrawTexturePro(*texture, {
            0, 0,
            (float)texture->width, (float)texture->height
        }, {
            10.f + (texture->width + 10.f) * i, GetScreenHeight() - texture->height - 10.f,
            texture->width * 1.f, texture->height * 1.f
        }, { 0, 0 }, 0.f, { 255, 255, 255, 191 });
    }
    
    renderTexture.lock()->BeginDrawingTo();
        auto& cards = gameController->GetCards();
        for (size_t idx = 0; idx < cards.size(); idx++)
        {
            auto card = cards[idx];
            DrawBeginningNode(card, { (float)idx, (cards.size() - 1) * narrowness });
        }

        clickablePositions.clear();
        for (auto pos : gameController->GetPlaceablePositions())
        {
            DrawFromType(CardType::EMPTY, {
                pos.x + abs(pos.y) * narrowness / 1.5f,
                float(abs(pos.y) + cards.size() - 1) * narrowness }, VerticalCardPos::CENTER);
            clickablePositions.push_back(pos);
        }
    renderTexture.lock()->EndDrawingTo();

    if (gameController->GetCurrentTurn() == Turn::PLAYER)
    {
        DrawText("Your Turn", 10, 10, 30, WHITE);
    }
    else
    {
        DrawText("Opponent's Turn", 10, 10, 30, WHITE);
    }
}

void GameWindow::OnDestroy()
{
    for (auto tex : cardTextures)
        ObjectManager::GetInstance()->DestroyObjectFromID(tex.second.lock()->GetID());

    ObjectManager::GetInstance()->DestroyObjectFromID(renderTexture.lock()->GetID());
}

bool GameWindow::DrawImageButton(CardType type)
{
    Texture *texture = nullptr;

    texture = &cardTextures[type].lock()->GetTexture();

    if (texture == nullptr) return false;

    ImVec2 imTexRes = { static_cast<float>(texture->width), static_cast<float>(texture->height) };
    ImTextureID id = (void*)&texture->id;

    return ImGui::ImageButton(id, imTexRes);
}

void GameWindow::OnUI()
{
    auto gameController = MGetComponent(GameController);

    if (gameController->IsGameOver())
    {
        ImGui::Begin("Message", nullptr, ImGuiWindowFlags_NoMove);
            ImGui::Text(gameController->GetOverReason().c_str());
            if (ImGui::Button("OK"))
            {
                GameManager::GetInstance()->ChangeScene("res/scenes/main_menu.json");
            }
        ImGui::End();
        return;
    }

    Texture& texture = renderTexture.lock()->GetRenderTexture().texture;
    auto& cards = gameController->GetCards();

    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    const ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;
    const ImVec2 windowSize {
        static_cast<float>(texture.width) + windowPadding.x * 2,
        static_cast<float>(texture.height) + windowPadding.y * 2
    };
    const ImVec2 windowPos {
        displaySize.x / 2 - windowSize.x / 2,
        displaySize.y / 2 - windowSize.y / 2
    };

    static Vec2i positionForPlacing { 0, 0 };

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5.f);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("GameWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoDocking);
        ImGui::Image((void*)&texture.id,
            { static_cast<float>(texture.width), static_cast<float>(texture.height) });
        const ImVec2 mousePosAbsolute = ImGui::GetMousePos();
        const ImVec2 screenPosAbsolute = ImGui::GetItemRectMin();
        const ImVec2 mousePosRelative {
            mousePosAbsolute.x - screenPosAbsolute.x,
            mousePosAbsolute.y - screenPosAbsolute.y
        };

        for (const auto& pos : clickablePositions)
        {
            if (CheckCollisionPointRec({ mousePosRelative.x, mousePosRelative.y }, {
                (pos.x + abs(pos.y) * narrowness / 1.5f) * scaleTo.x,
                (float(abs(pos.y) + cards.size() - 1) * narrowness) * scaleTo.y,
                scaleTo.x, scaleTo.y
            }) && ImGui::IsMouseReleased(ImGuiMouseButton_Left)
               && !ImGui::IsPopupOpen("card_select_popup"))
            {
                ImGui::OpenPopup("card_select_popup");
                positionForPlacing = pos;
            }
        }
        
        if (ImGui::BeginPopup("card_select_popup"))
        {
            auto placeableCards = gameController->GetPlaceableCards(positionForPlacing);
            auto& playerDeck = gameController->GetDeck();
            std::vector<CardType> deckSorted(playerDeck.size());
            std::vector<CardType> crossSection;

            std::sort(placeableCards.begin(), placeableCards.end());
            std::partial_sort_copy(playerDeck.cbegin(), playerDeck.cend(),
                deckSorted.begin(), deckSorted.end());

            std::set_intersection(
                placeableCards.begin(), placeableCards.end(),
                deckSorted.begin(), deckSorted.end(), std::back_inserter(crossSection)
            );

            if (crossSection.empty())
                ImGui::Text("No Options Available :P");

            for (const auto& type : crossSection)
            {
                ImGui::SameLine();
                if (DrawImageButton(type))
                {
                    gameController->PlaceCardFromDeckIndex(std::find(playerDeck.begin(), playerDeck.end(), type) - playerDeck.begin(),
                        positionForPlacing);
                    ImGui::CloseCurrentPopup();
                    positionForPlacing = { 0, 0 };
                }
            }

            ImGui::EndPopup();
        }
    ImGui::End();
    ImGui::PopStyleVar();
}

void GameWindow::DrawBeginningNode(BeginningNode<CardType> top, Vec2f idx)
{
    DrawFromType(top.val, idx, VerticalCardPos::CENTER);

    if (top.topNext != nullptr)
        DrawNode(*top.topNext, { idx.x - narrowness / 1.5f, idx.y - narrowness }, VerticalCardPos::TOP);

    if (top.bottomNext != nullptr)
        DrawNode(*top.bottomNext, { idx.x - narrowness / 1.5f, idx.y + narrowness }, VerticalCardPos::BOTTOM);
}

void GameWindow::DrawNode(Node<CardType> top, Vec2f idx, VerticalCardPos vertPos)
{
    DrawFromType(top.val, idx, vertPos);

    if (top.next != nullptr)
        DrawNode(*top.next,
            { idx.x - .5f, vertPos == VerticalCardPos::BOTTOM ? idx.y + narrowness : idx.y - narrowness }, vertPos);
}

void GameWindow::DrawFromType(CardType type, Vec2f index, VerticalCardPos vertPos)
{
    /*
        x -> index
        y -> floor
    */
    Vec2f actualPos = {
        index.x * scaleTo.x,
        index.y * scaleTo.y
    };

    Vec2i texRes = cardTextures[type == CardType::STATE_1_0 ? CardType::STATE_0_1 : type].lock()->GetSize();

    Rectf dest = {
        actualPos.x + scaleTo.x / 2,
        actualPos.y + scaleTo.y / 2,
        scaleTo.x,
        scaleTo.y
    };
    
    Recti source = {
        0, 0,
        texRes.x,
        texRes.y
    };

    Vec2f origin = {
        scaleTo.x / 2.f,
        scaleTo.y / 2.f
    };

    if (type == CardType::STATE_1_0)
        cardTextures[CardType::STATE_0_1].lock()->Draw(source, dest, 0.f, origin);
    else if (type == CardType::STATE_0_1)
        cardTextures[type].lock()->Draw(source, dest, 180.f, origin);
    else cardTextures[type].lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
}