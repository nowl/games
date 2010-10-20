DropStack = {}
DestroyedDropStack = {}

function mob_drop_energy (mob)
   drop = {obj = bl.object.new(), image = nil}
   bl.state.push(main_state, drop.obj)
   bl.object.update_append(drop.obj, "drop_update")
   bl.object.render_append(drop.obj, "drop_draw")
   drop.x = mob.x
   drop.y = mob.y
   drop.speed = 5
   drop.px = drop.x
   drop.py = drop.y
   drop.start_tick = bl.manager.cur_tick()
   DropStack[drop.obj] = drop

   bl.sound.play("mob-drop")
end

function drop_update (gm, obj, ticks)
   o = DropStack[obj]
   o.y = o.y + o.speed

   if o.y > 600 then
      DestroyedDropStack[#DestroyedDropStack + 1] = o.obj
      bl.state.remove(main_state, obj)
      DropStack[obj] = nil
      return
   end

   -- test for collision with player
   local collisionWithPlayer = test_drop_collision_with_player(o)
   if collisionWithPlayer then
      player.energy = player.energy + 100
      if player.energy > player.energy_max then
         player.energy = player.energy_max
      end
      bl.sound.play("energy-gain")         

      DestroyedDropStack[#DestroyedDropStack + 1] = o.obj
      bl.state.remove(main_state, o.obj)
      DropStack[obj] = nil
   end

   o.px = o.px
   o.py = o.y

   o.image = bl.image.set_get("drop-seq1", ticks - drop.start_tick)
end

function drop_draw (gm, obj, int)
   o = DropStack[obj]
   bl.draw.image(o.image, o.x, o.y)
end


gc_drops_tick_count = 0
function gc_drops (gm, obj, ticks)
   if gc_drops_tick_count <= 500 then
      gc_drops_tick_count = gc_drops_tick_count + 1
      return
   end

   gc_drops_tick_count = 0

   if #DestroyedDropStack >= 1 then
      for i = 1, #DestroyedDropStack do
         bl.object.destroy(DestroyedDropStack[i])
      end
      DestroyedDropStack = {}
   end
end

function drop_kill ()
   for drop_obj, drop in pairs(DropStack) do
      DestroyedDropStack[#DestroyedDropStack + 1] = drop_obj
      bl.state.remove(main_state, drop_obj)
      DropStack[drop_obj] = nil
   end
end

function test_drop_collision_with_player (o)
   local traj_poly = {o.px+6, o.py+10,
                           o.px+25, o.py+10,
                           o.x+5, o.y+24,
                           o.x+25, o.y+24}
   local collisions = {}
   collision = bl.object.collide_polys(traj_poly, player_get_poly())
   if collision then
      return true
   else
      return false
   end
end
