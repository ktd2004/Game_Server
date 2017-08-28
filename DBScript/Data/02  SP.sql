USE summon_data;


DROP PROCEDURE IF EXISTS `SP_SelectCharacter`;
DELIMITER //
CREATE PROCEDURE `SP_SelectCharacter`()
BEGIN
  SELECT * FROM `character`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectCharacterLevel`;
DELIMITER //
CREATE PROCEDURE `SP_SelectCharacterLevel`()
BEGIN
  SELECT * FROM character_level;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMonster`()
BEGIN
  SELECT * FROM monster;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMonsterLevel`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMonsterLevel`()
BEGIN
  SELECT * FROM monster_level;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectDice`;
DELIMITER //
CREATE PROCEDURE `SP_SelectDice`()
BEGIN
  SELECT * FROM dice;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectSkill`;
DELIMITER //
CREATE PROCEDURE `SP_SelectSkill`()
BEGIN
  SELECT * FROM skill;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMapData`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMapData`()
BEGIN
  SELECT * FROM map_data;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMapIdx`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMapIdx`()
BEGIN
  SELECT * FROM map_idx;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMapRandStat`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMapRandStat`()
BEGIN
  SELECT * FROM map_rand_stat;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectMapProvideMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectMapProvideMonster`()
BEGIN
  SELECT * FROM map_provide_monster;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectStageData`;
DELIMITER //
CREATE PROCEDURE `SP_SelectStageData`()
BEGIN
  SELECT * FROM stage_data;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectStageAI`;
DELIMITER //
CREATE PROCEDURE `SP_SelectStageAI`()
BEGIN
  SELECT * FROM stage_ai;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPullMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPullMonster`()
BEGIN
  SELECT * FROM pull_monster;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectItem`;
DELIMITER //
CREATE PROCEDURE `SP_SelectItem`()
BEGIN
  SELECT * FROM item;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectReinforceMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectReinforceMonster`()
BEGIN
  SELECT * FROM reinforce_monster;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectEvolveMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectEvolveMonster`()
BEGIN
  SELECT * FROM evolve_monster;
END//
DELIMITER ;


/* player_default_data */
DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultInfo`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultInfo`()
BEGIN
  SELECT * FROM player_default_info;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultCharacter`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultCharacter`()
BEGIN
  SELECT * FROM player_default_character;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultMonster`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultMonster`()
BEGIN
  SELECT * FROM player_default_monster;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultTeam`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultTeam`()
BEGIN
  SELECT * FROM player_default_team;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultInventory`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultInventory`()
BEGIN
  SELECT * FROM player_default_inventory;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerDefaultMap`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerDefaultMap`()
BEGIN
  SELECT * FROM player_default_map;
END//
DELIMITER ;
/* end player_default_data */
