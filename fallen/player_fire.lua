FireStack = {}
DestroyedFireStack = {}

FireCost = 25

function player_fire()
   if player.energy <= FireCost or player.fire_cooldown ~= 0 then
      return
   end

   player.energy = player.energy - FireCost
   player.fire_cooldown = player.fire_cooldown_reset

   fire = {obj = bl.object.new(), image = nil}
   bl.state.push(main_state, fire.obj)
   bl.object.update_append(fire.obj, "player_fire_update")
   bl.object.render_append(fire.obj, "player_fire_draw")
   fire.x = player.x
   fire.y = player.y
   fire.px = fire.x
   fire.py = fire.y
   fire.start_tick = bl.manager.cur_tick()
   FireStack[fire.obj] = fire

   bl.sound.play("fire")
end

function mob_fire(mob, speed)
   fire = {obj = bl.object.new(), image = nil}
   bl.state.push(main_state, fire.obj)
   bl.object.update_append(fire.obj, "mob_fire_update")
   bl.object.render_append(fire.obj, "mob_fire_draw")
   fire.x = mob.x
   fire.y = mob.y
   fire.speed = speed
   fire.px = fire.x
   fire.py = fire.y
   fire.start_tick = bl.manager.cur_tick()
   FireStack[fire.obj] = fire
end

function player_fire_update (gm, obj, ticks)
   o = FireStack[obj]
   o.y = o.y - 10

   if o.y < 0 then
      DestroyedFireStack[#DestroyedFireStack + 1] = o.obj
      bl.state.remove(main_state, obj)
      FireStack[obj] = nil
      return
   end

   -- test for collisions
   local collisionMobs = test_fire_collision_with_mobs(o)
   if #collisionMobs > 0 then
      for c = 1, #collisionMobs do
         collisionMobs[c].hp = 0
         player.score = player.score + collisionMobs[c].score_value
         bl.sound.play("mob-explode")
      end
      
      DestroyedFireStack[#DestroyedFireStack + 1] = o.obj
      bl.state.remove(main_state, o.obj)
      FireStack[obj] = nil
      return
   end

   o.px = o.px
   o.py = o.y

   o.image = bl.image.set_get("player-bullet-seq", ticks - fire.start_tick)
end

function player_fire_draw (gm, obj, int)
   o = FireStack[obj]
   bl.draw.image(o.image, o.x, o.y)
end

function fire_cooldown (gm, obj, ticks)
   player.fire_cooldown = player.fire_cooldown - 1
   if player.fire_cooldown <= 0 then
      player.fire_cooldown = 0
   end
end

function mob_fire_update (gm, obj, ticks)
   o = FireStack[obj]
   o.y = o.y + o.speed

   if o.y > 600 then
      DestroyedFireStack[#DestroyedFireStack + 1] = o.obj
      bl.state.remove(main_state, obj)
      FireStack[obj] = nil
      return
   end

   -- test for collision with player
   local collisionWithPlayer = test_fire_collision_with_player(o)
   if collisionWithPlayer then
      player.energy = player.energy - 200

      DestroyedFireStack[#DestroyedFireStack + 1] = o.obj
      bl.state.remove(main_state, o.obj)
      FireStack[obj] = nil

      bl.sound.play("player-hit")
   end

   o.px = o.px
   o.py = o.y

   o.image = bl.image.set_get("mob-bullet-seq", ticks - fire.start_tick)
end

function mob_fire_draw (gm, obj, int)
   o = FireStack[obj]
   bl.draw.image(o.image, o.x, o.y)
end

function test_fire_collision_with_mobs (o)
   local fire_traj_poly = {o.px+32/2-5, o.py+32/2+5,
                           o.px+32/2+5, o.py+32/2+5,
                           o.x+32/2+5, o.y+32/2-5,
                           o.x+32/2-5, o.y+32/2-5}
   local collisions = {}
   local count = 1
   for k, v in pairs(WaveMobs) do      
      collision = bl.object.collide_polys(fire_traj_poly, mob_get_poly(v))
      if collision then
         collisions[count] = v
         count = count + 1
      end
   end

   return collisions
end

function test_fire_collision_with_player (o)
   local fire_traj_poly = {o.px+32/2-5, o.py+32/2+5,
                           o.px+32/2+5, o.py+32/2+5,
                           o.x+32/2+5, o.y+32/2-5,
                           o.x+32/2-5, o.y+32/2-5}
   local collisions = {}
   collision = bl.object.collide_polys(fire_traj_poly, player_get_poly())
   if collision then
      return true
   else
      return false
   end
end

gc_bullets_tick_count = 0
function gc_bullets (gm, obj, ticks)
   if gc_bullets_tick_count <= 500 then
      gc_bullets_tick_count = gc_bullets_tick_count + 1
      return
   end

   gc_bullets_tick_count = 0

   if #DestroyedFireStack >= 1 then
      for i = 1, #DestroyedFireStack do
         bl.object.destroy(DestroyedFireStack[i])
      end
      DestroyedFireStack = {}
   end
end

function fire_kill ()
   for fire_obj, fire in pairs(FireStack) do
      DestroyedFireStack[#DestroyedFireStack + 1] = fire_obj
      bl.state.remove(main_state, fire_obj)
      FireStack[fire_obj] = nil
   end
end