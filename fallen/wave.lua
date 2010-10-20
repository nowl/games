WaveMobs = {}
DestroyedWaveMobs = {}

math.randomseed(os.time())

mob_types = {{["NORMAL"] = "red-mob", ["FIRING"] = "red-mob-firing", ["DYING"] = "red-mob-explosion"},
             {["NORMAL"] = "green-mob", ["FIRING"] = "green-mob-firing", ["DYING"] = "green-mob-explosion"}}

function build_wave (wave_number)
   for c = 1, 10+2*wave_number do
      mob = {obj = bl.object.new(),
             state = "NORMAL",
             type = math.random(2),
             x = math.random(799-32),
             y = math.random(102, 398),
             dx = math.random()*6 - 3,
             dy = math.random()*6 - 3,
             chance_to_fire = 0.01,
             chance_to_drop_energy_when_destroyed = 0.1,
             chance_to_change_direction = 0.001,
			 firing_speed = math.random()*4 + 1,
             is_firing = 0,
             hp = 100,
			 image = nil,
             death_counter = 0,
             score_value = 5*wave_number,
          }

      bl.state.push(main_state, mob.obj)      
      bl.object.render_append(mob.obj, "draw_mob")
      bl.object.update_append(mob.obj, "update_mob")
      
      WaveMobs[mob.obj] = mob
   end
end

function mob_get_poly(mob)
   if mob.state == "DYING" then
      return {0, 0, 0, 0}
   else
      return {mob.x+4, mob.y+4,
              mob.x+32-4, mob.y+4,
              mob.x+32-4, mob.y+32-4,
              mob.x+4, mob.y+32-4}
   end
end

function draw_mob (gm, obj, int)
   mob = WaveMobs[obj]
   if mob.image then
      bl.draw.image(mob.image, mob.x, mob.y)
   end
end

function update_mob (gm, obj, ticks)
   mob = WaveMobs[obj]

   if mob.state == "NORMAL" or mob.state == "FIRING" then
      mob.x = mob.x + mob.dx
      mob.y = mob.y + mob.dy
      if mob.x < 0 or mob.x+32 > 800 then
         mob.dx = -mob.dx
      end
      if mob.y < 60 or mob.y+32 > 400 then
         mob.dy = -mob.dy
      end
      
      if math.random() < mob.chance_to_change_direction then
         mob.dx = math.random()*6 - 3
         mob.dy = math.random()*6 - 3
      end

      if mob.is_firing > 0 then
         mob.is_firing = mob.is_firing - 1
      else
         if math.random() < mob.chance_to_fire then
            mob.state = "FIRING"
            mob.is_firing = 30
            mob_fire(mob, mob.firing_speed)
         else
            mob.state = "NORMAL"
         end
      end
      
      if mob.hp <= 0 then
         mob.state = "DYING"
         mob.death_counter = ticks

         -- random chance to drop energy
         if math.random() < mob.chance_to_drop_energy_when_destroyed then
			mob_drop_energy(mob)
         end
      end
   elseif mob.state == "DYING" then
      if ticks - mob.death_counter >= 20*2 then
         DestroyedWaveMobs[#DestroyedWaveMobs + 1] = mob.obj
         bl.state.remove(main_state, mob.obj)
         WaveMobs[mob.obj] = nil
      end
   end
   
   local image = mob_types[mob.type][mob.state]
   mob.image = bl.image.set_get(image, ticks - mob.death_counter)
end

gc_mobs_tick_count = 0
function gc_mobs (gm, obj, ticks)
   if gc_mobs_tick_count <= 500 then
      gc_mobs_tick_count = gc_mobs_tick_count + 1
      return
   end

   gc_mobs_tick_count = 0
   
   if #DestroyedWaveMobs >= 1 then
      for i = 1, #DestroyedWaveMobs do
         bl.object.destroy(DestroyedWaveMobs[i])
      end
      DestroyedWaveMobs = {}
   end
end

function wave_pause ()
   for mob_obj, mob in pairs(WaveMobs) do
      bl.object.update_clear(mob_obj)
   end
end

function wave_play ()
   for mob_obj, mob in pairs(WaveMobs) do
      bl.object.update_clear(mob_obj)
      bl.object.update_append(mob_obj, "update_mob")
   end
end

function kill_mobs ()
   for mob_obj, mob in pairs(WaveMobs) do
      bl.object.destroy(mob_obj)
   end
   WaveMobs = {}
end

function count_live_mobs ()
   local count = 0
   for k, v in pairs(WaveMobs) do
      count = count + 1
   end
   return count
end