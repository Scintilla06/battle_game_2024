#include "curve_bullet.h"
#include "battle_game/core/game_core.h"

namespace battle_game::bullet {

CurveBullet::CurveBullet(GameCore *core,
                         uint32_t id,
                         uint32_t unit_id,
                         uint32_t player_id,
                         glm::vec2 position,
                         float rotation,
                         float damage_scale,
                         glm::vec2 velocity,
                         float angular_velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity),
      angular_velocity_(angular_velocity) {}

void CurveBullet::Update() {
  // 旋转速度向量
  velocity_ = Rotate(velocity_, angular_velocity_ * kSecondPerTick);
  
  // 更新位置
  position_ += velocity_ * kSecondPerTick;
  
  // 更新旋转角度以朝向运动方向
  rotation_ = std::atan2(velocity_.y, velocity_.x) - glm::radians(90.0f);

  // 碰撞检测
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) continue;
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f);
      should_die = true;
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

void CurveBullet::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

}