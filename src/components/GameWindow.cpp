#include "./GameWindow.hpp"

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
        int(scaleTo.y) * int(cards.size() * 2 - 1) /*- int(scaleTo.y - scaleTo.y * .75f) * int(cards.size() * 2 - 1)*/
    });

    state.lock()->LoadFromFile("res/img/state.png");
    andZero.lock()->LoadFromFile("res/img/and_0.png");
    andOne.lock()->LoadFromFile("res/img/and_1.png");
    orZero.lock()->LoadFromFile("res/img/or_0.png");
    orOne.lock()->LoadFromFile("res/img/or_1.png");
    xorZero.lock()->LoadFromFile("res/img/xor_0.png");
    xorOne.lock()->LoadFromFile("res/img/xor_1.png");

    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {0, -1});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {1, -1});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {2, -1});
    // MGetComponent(GameController)->PlaceCard(CardType::OR_0, {3, -1});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {4, -1});

    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {0, -2});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {1, -2});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {2, -2});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {3, -2});

    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {0, -3});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {1, -3});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {2, -3});

    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {0, -4});
    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {1, -4});

    MGetComponent(GameController)->PlaceCard(CardType::OR_0, {0, -5});
}

void GameWindow::OnUpdate()
{

    renderTexture.lock()->BeginDrawingTo();
        auto cards = MGetComponent(GameController)->GetCards();
        for (size_t idx = 0; idx < cards.size(); idx++)
        {
            auto card = cards[idx];
            DrawBeginningNode(card, { (float)idx, cards.size() - 1 });
        }
    renderTexture.lock()->EndDrawingTo();

    if (MGetComponent(GameController)->GetCurrentTurn() == Turn::YOUR)
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
}

void GameWindow::OnUI()
{
    Texture& texture = renderTexture.lock()->GetRenderTexture().texture;
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    const ImVec2 windowSize { static_cast<float>(texture.width), static_cast<float>(texture.height) };
    const ImVec2 windowPos {
        displaySize.x / 2 - windowSize.x / 2,
        displaySize.y / 2 - windowSize.y / 2
    };

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("GameWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);
        ImGui::Image((void*)&texture.id,
            { static_cast<float>(texture.width), static_cast<float>(texture.height) });
    ImGui::End();
    ImGui::PopStyleVar();
}

void GameWindow::DrawBeginningNode(BeginningNode<CardType> top, Vec2f idx)
{
    DrawFromType(top.val, idx, VerticalCardPos::CENTER);

    if (top.topNext != nullptr)
        DrawNode(*top.topNext, { idx.x - .5f, idx.y - .75f }, VerticalCardPos::TOP);

    if (top.bottomNext != nullptr)
        DrawNode(*top.bottomNext, { idx.x - .5f, idx.y + .75f }, VerticalCardPos::BOTTOM);
}

void GameWindow::DrawNode(Node<CardType> top, Vec2f idx, VerticalCardPos vertPos)
{
    DrawFromType(top.val, idx, vertPos);

    if (top.next != nullptr)
        DrawNode(*top.next,
            { idx.x - .5f, vertPos == VerticalCardPos::BOTTOM ? idx.y + .75f : idx.y - .75f }, vertPos);
    // else DrawFromType(CardType::EMPTY, { idx.x - .5f, idx.y + .75f }, VerticalCardPos::CENTER);
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
        DrawRectangleV(actualPos, scaleTo, WHITE);
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