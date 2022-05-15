#include "./GameWindow.hpp"
#include "../GameManager.hpp"

void GameWindow::OnCreate()
{
    renderTexture = ObjectManager::GetInstance()->CreateObject<TextureObject>();

    state = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    andZero = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    andOne = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    orZero = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    orOne = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    xorZero = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    xorOne = ObjectManager::GetInstance()->CreateObject<TextureObject>();

    auto cards = MGetComponent(GameController)->GetCards();

    renderTexture.lock()->LoadEmpty({
        int(scaleTo.x) * int(cards.size()),
        int(scaleTo.y * narrowness) * int(cards.size() * 2 - 1) + int(scaleTo.y * .30f)
    });

    state.lock()->LoadFromFile("res/img/themes/cards/state-0-1.png");
    andZero.lock()->LoadFromFile("res/img/themes/cards/and_0.png");
    andOne.lock()->LoadFromFile("res/img/themes/cards/and_1.png");
    orZero.lock()->LoadFromFile("res/img/themes/cards/or_0.png");
    orOne.lock()->LoadFromFile("res/img/themes/cards/or_1.png");
    xorZero.lock()->LoadFromFile("res/img/themes/cards/xor_0.png");
    xorOne.lock()->LoadFromFile("res/img/themes/cards/xor_1.png");
}

void GameWindow::OnUpdate()
{
    auto gameController = MGetComponent(GameController);
    
    if (gameController->IsGameOver())
        return;

    renderTexture.lock()->BeginDrawingTo();
        auto cards = gameController->GetCards();
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
    ObjectManager::GetInstance()->DestroyObjectFromID(state.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(andZero.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(andOne.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(orZero.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(orOne.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(xorZero.lock()->GetID());
    ObjectManager::GetInstance()->DestroyObjectFromID(xorOne.lock()->GetID());

    ObjectManager::GetInstance()->DestroyObjectFromID(renderTexture.lock()->GetID());
}

bool GameWindow::DrawImageButton(CardType type)
{
    Texture *texture = nullptr;

    switch(type)
    {
    case CardType::AND_0:
        texture = &andZero.lock()->GetTexture();
        break;
    case CardType::AND_1:
        texture = &andOne.lock()->GetTexture();
        break;
    case CardType::OR_0:
        texture = &orZero.lock()->GetTexture();
        break;
    case CardType::OR_1:
        texture = &orOne.lock()->GetTexture();
        break;
    case CardType::XOR_0:
        texture = &xorZero.lock()->GetTexture();
        break;
    case CardType::XOR_1:
        texture = &xorOne.lock()->GetTexture();
        break;
    case CardType::STATE_1_0:
        texture = &state.lock()->GetTexture();
        break;
    case CardType::STATE_0_1:
        texture = &state.lock()->GetTexture();
        break;
    case CardType::EMPTY: {}
    }

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
    auto cards = gameController->GetCards();

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
        
        if (ImGui::BeginPopup("card_select_popup"))
        {
            const auto cards = gameController->GetDeck();
            for (unsigned short i = 0; i < static_cast<unsigned short>(cards.size()); i++)
            {
                auto type = cards[i];

                ImGui::SameLine();
                if (DrawImageButton(type))
                {
                    gameController->PlaceCardFromDeckIndex(i, positionForPlacing);
                    ImGui::CloseCurrentPopup();
                    positionForPlacing = { 0, 0 };
                }
            }
            ImGui::EndPopup();
        }
        
        for (auto pos : clickablePositions)
        {
            if (CheckCollisionPointRec({ mousePosRelative.x, mousePosRelative.y }, {
                (pos.x + abs(pos.y) * narrowness / 1.5f) * scaleTo.x,
                (float(abs(pos.y) + cards.size() - 1) * narrowness) * scaleTo.y,
                scaleTo.x, scaleTo.y
            }) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                ImGui::OpenPopup("card_select_popup");
                positionForPlacing = pos;
            }
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

    if (type == CardType::EMPTY)
    {
        DrawRectangleV(actualPos, const_cast<Vec2f&>(scaleTo), WHITE);
    }

    Vec2i texRes {};

    switch(type)
    {
    case CardType::AND_0:
        texRes = andZero.lock()->GetSize();
        break;
    case CardType::AND_1:
        texRes = andOne.lock()->GetSize();
        break;
    case CardType::OR_0:
        texRes = orZero.lock()->GetSize();
        break;
    case CardType::OR_1:
        texRes = orOne.lock()->GetSize();
        break;
    case CardType::XOR_0:
        texRes = xorZero.lock()->GetSize();
        break;
    case CardType::XOR_1:
        texRes = xorOne.lock()->GetSize();
        break;
    case CardType::STATE_1_0:
        texRes = state.lock()->GetSize();
        break;
    case CardType::STATE_0_1:
        texRes = state.lock()->GetSize();
        break;
    case CardType::EMPTY: {}
    }

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

    switch(type)
    {
    case CardType::AND_0:
        andZero.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::AND_1:
        andOne.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::OR_0:
        orZero.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::OR_1:
        orOne.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::XOR_0:
        xorZero.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::XOR_1:
        xorOne.lock()->Draw(source, dest, vertPos == VerticalCardPos::TOP ? 180.f : 0.f, origin);
        break;
    case CardType::STATE_1_0:
        state.lock()->Draw(source, dest, 0.f, origin);
        break;
    case CardType::STATE_0_1:
        state.lock()->Draw(source, dest, 180.f, origin);
        break;
    case CardType::EMPTY: {}
    }
}