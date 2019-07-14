# Tanks Vs Zombies
Tanks vs Zombies using Paper2D and GameplayAbilites

Video showing some features in action: https://youtu.be/65oHJNchafo

Based on the TanksVsZombies stream by Epic Games

This project sample is made for single player and might not implement the best practices but it contains:

- Different types of projectile abilities that use GameplayEvents to detect Hits:
  * Normal projectile that damages one enemy
  * AoE projectiles that damage more than one enemy on hit or on explode
  * A fire projectile that does fire damage over time
  * An ice projectile that slows target hit
  
- Different types of abilities called from diffrent places in the code using different cooldowns
  * An abiltiy on the tank that kills zombies on overlap
  * The zombies basic attack abiltiy
  
- An example of damage modification:
  * Using defense power to reduce damage taken
  * Targets taking extra damage from FireDamage type
  
- GameplayCues that shows the effects of abilities
  * Targets on fire are tainted red
  * Frozen targets are tainted blue
  * All effects and cues would work on both tanks and zombies so it's possible to make AI tanks or make zombies use fire and ice abilities/ projectiles
  
- Using DataTables and CurveTables for Data Driven gameplay
  * Curve table to init the stats of diffrent types of pawns
  * Data tables to manages Gameplay Tags and Cue Tags
  
- An example of using different types of AttributeSets in the same class

- An example of making a custom TargetActor to get the target data wanted for your ability

- An example of using Paprer2D with C++

- Possible additions to this project that you can work on by adding extra abilties and effects:
  * A heal or regen ability/effect
  * Any passive effect that presists through the game
  * Making different types of zombies that can shoot projectiles just by changing the ZombieAttack abiltiy
  * Can also have different types of Tanks and Zombies by playing with the StartingStats Curve table ex: zombies with a lot of health that walks slowly

Known bugs: sometimes the projectile explodes prematurely

Special thanks to Wopping from the Unreal Slackers discord channel for his constant help
