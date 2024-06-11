#pragma once
#include <string>

// アクションの基底クラス
class Action {
public:
    // アクションを実行する純粋仮想関数
    virtual bool execute() = 0;
    // アクションの名前を取得する純粋仮想関数
    virtual std::string name() = 0;
};

// メモ　＊＊＊＊＊＊＊＊＊＊＊＊
// compositeパターン **************************


// 椅子から立つアクション
class Action1 : public Action {
public:
    bool execute() override;
    std::string name() override { return "椅子から立つ"; }
};

// 自販機に移動するアクション
class Action2 : public Action {
public:
    bool execute() override;
    std::string name() override { return "自販機に移動する"; }
};

// 麦茶を買うアクション
class Action3 : public Action {
public:
    bool execute() override;
    std::string name() override { return "麦茶を買う"; }
};

// 水を買うアクション
class Action4 : public Action {
public:
    bool execute() override;
    std::string name() override { return "水を買う"; }
};

// 自席に移動するアクション
class Action5 : public Action {
public:
    bool execute() override;
    std::string name() override { return "自席に移動する"; }
};

// 椅子に座るアクション
class Action6 : public Action {
public:
    bool execute() override;
    std::string name() override { return "椅子に座る"; }
};
