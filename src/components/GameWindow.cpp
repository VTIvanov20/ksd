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

    renderTexture.lock()->LoadEmpty({
        int(scaleTo.x + spacing.x) * 6,
        int(scaleTo.x + spacing.x) * 11
    });

    state.lock()->LoadFromFile("res/img/state.png");
    andZero.lock()->LoadFromFile("res/img/and_0.png");
    andOne.lock()->LoadFromFile("res/img/and_1.png");
    orZero.lock()->LoadFromFile("res/img/or_0.png");
    orOne.lock()->LoadFromFile("res/img/or_1.png");
    xorZero.lock()->LoadFromFile("res/img/xor_0.png");
    xorOne.lock()->LoadFromFile("res/img/xor_1.png");
}

void GameWindow::OnUpdate()
{
    auto cards = MGetComponent(GameController)->GetCards();
    renderTexture.lock()->BeginDrawingTo();
        for (size_t idx = 0; idx < cards.size(); idx++)
        {
            auto card = cards[idx];
            DrawBeginningNode(card, { (float)idx, 1.f });
        }
    renderTexture.lock()->EndDrawingTo();
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
    ImGui::Begin("GameWindow");
        ImGui::Image((void*)&texture.id,
            { static_cast<float>(texture.width), static_cast<float>(texture.height) });
    ImGui::End();
}

void GameWindow::DrawBeginningNode(BeginningNode<CardType> top, Vec2f idx)
{
    DrawFromType(top.val, idx, {});

    if (top.topR != nullptr)
        DrawNode(*top.topR, { idx.x, idx.y + 1 }, VerticalCardPos::TOP);
    if (top.bottomR != nullptr)
        DrawNode(*top.bottomR, { idx.x, idx.y - 1 }, VerticalCardPos::BOTTOM);
}

void GameWindow::DrawNode(Node<CardType> top, Vec2f idx, VerticalCardPos vertPos)
{
    DrawFromType(top.val, idx, {});

    if (top.right != nullptr)
        DrawNode(*top.right,
            { idx.x + .5f, vertPos == VerticalCardPos::BOTTOM ? idx.y - 1 : idx.y + 1 }, vertPos);
}

void GameWindow::DrawFromType(CardType type, Vec2f index, Vec2f spacing)
{
    /*
        x -> index
        y -> floor
    */
    Vec2f actualPos = {
        index.x * scaleTo.x + spacing.x,
        index.y * scaleTo.y + spacing.y
    };

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
    }

    Rectf dest = {
        actualPos.x + texRes.x / 2,
        actualPos.y + texRes.y / 2,
        scaleTo.x,
        scaleTo.y
    };
    
    Recti source = {
        0, 0,
        texRes.x,
        texRes.y
    };

    Vec2f origin = {
        texRes.x / 2.f,
        texRes.y / 2.f
    };

    switch(type)
    {
    case CardType::AND_0:
        andZero.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::AND_1:
        andOne.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::OR_0:
        orZero.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::OR_1:
        orOne.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::XOR_0:
        xorZero.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::XOR_1:
        xorOne.lock()->Draw(source, dest, index.x < 0 ? 180.f : 0.f, origin);
        break;
    case CardType::STATE_1_0:
        state.lock()->Draw(source, dest, 0.f, origin);
        break;
    case CardType::STATE_0_1:
        state.lock()->Draw(source, dest, 180.f, origin);
        break;
    }
}