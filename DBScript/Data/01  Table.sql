CREATE DATABASE `summon_data`;
USE summon_data;


DROP TABLE IF EXISTS `category`;
CREATE TABLE `category` (
  `iCategory` smallint(5) unsigned NOT NULL,
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `sComment` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iCategory`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `section`;
CREATE TABLE `section` (
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `sComment` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iCategory`, `iSection`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `character`;
CREATE TABLE `character` (
  `iCID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iPriceType` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iBuyPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iEventReward` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `character_level`;
CREATE TABLE `character_level` (
  `iCID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_1` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_1_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_2` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_2_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_3` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_3_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_4` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_4_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_5` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_5_value` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iCID`, `iLevel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `monster`;
CREATE TABLE `monster` (
  `iMID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iRare` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iPriceType` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iSellPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iBuyPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iGrant` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iEnchant` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `iEventReward` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `monster_level`;
CREATE TABLE `monster_level` (
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iMana` int(10) unsigned NOT NULL DEFAULT '0',
  `iMinAtk` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxAtk` int(10) unsigned NOT NULL DEFAULT '0',
  `iHP` int(10) unsigned NOT NULL DEFAULT '0',
  `iResExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_Value` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMID`, `iLevel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `character_string`;
CREATE TABLE `character_string` (
  `iID` int(10) unsigned NOT NULL,
  `iRegion` smallint(5) unsigned NOT NULL,
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL,
  `sComment` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `sTooltip` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iID`, `iRegion`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `system_string`;
CREATE TABLE `system_string` (
  `iID` int(10) unsigned NOT NULL,
  `iRegion` smallint(5) unsigned NOT NULL,
  `sString` varchar(255) CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iID`, `iRegion`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `dice`;
CREATE TABLE `dice` (
  `iDID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iBuyPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iEventReward` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iSkill_1` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_1_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_2` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_2_value` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_3` int(10) unsigned NOT NULL DEFAULT '0',
  `iSkill_3_value` int(10) unsigned NOT NULL DEFAULT '0',
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iDID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `skill`;
CREATE TABLE `skill` (
  `iSID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iSID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `map_data`;
CREATE TABLE `map_data` (
  `iMapID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iPriceType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iPlayPeople` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iIdxCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iRoundBonus` int(10) unsigned NOT NULL DEFAULT '0',
  `sMapFile` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMapID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `map_idx`;
CREATE TABLE `map_idx` (
  `iMapID` int(10) unsigned NOT NULL,
  `iIdx` smallint(5) unsigned NOT NULL,
  `iIdxType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMapID`, `iIdx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `map_rand_stat`;
CREATE TABLE `map_rand_stat` (
  `iMapID` int(10) unsigned NOT NULL,
  `iStage` smallint(5) unsigned NOT NULL,
  `iType1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iID1` int(10) unsigned NOT NULL DEFAULT '0',
  `iValue1` int(10) NOT NULL DEFAULT '0',
  `iType2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iID2` int(10) unsigned NOT NULL DEFAULT '0',
  `iValue2` int(10) NOT NULL DEFAULT '0',
  `iType3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iID3` int(10) unsigned NOT NULL DEFAULT '0',
  `iValue3` int(10) NOT NULL DEFAULT '0',
  `iType4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iID4` int(10) unsigned NOT NULL DEFAULT '0',
  `iValue4` int(10) NOT NULL DEFAULT '0',
  `iType5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iID5` int(10) unsigned NOT NULL DEFAULT '0',
  `iValue5` int(10) NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMapID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `map_provide_monster`;
CREATE TABLE `map_provide_monster` (
  `iMapID` int(10) unsigned NOT NULL,
  `iUserType` tinyint(1) unsigned NOT NULL,
  `iMinLv` smallint(5) unsigned NOT NULL,
  `iMaxLv` smallint(5) unsigned NOT NULL,
  `sData` TEXT CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMapID`, `iUserType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `stage_data`;
CREATE TABLE `stage_data` (
  `iStage` smallint(5) unsigned NOT NULL,
  `iMapID` int(10) unsigned NOT NULL,
  `iLimitRound` smallint(5) unsigned NOT NULL,
  `iRewardGold` int(10) unsigned NOT NULL,
  `iRewardExp` int(10) unsigned NOT NULL,
  `iRewardMID1` int(10) unsigned NOT NULL DEFAULT '0',
  `fProb1` double(10,3) unsigned NOT NULL DEFAULT '0',
  `iRewardMID2` int(10) unsigned NOT NULL DEFAULT '0',
  `fProb2` double(10,3) unsigned NOT NULL DEFAULT '0',
  `iRewardMID3` int(10) unsigned NOT NULL DEFAULT '0',
  `fProb3` double(10,3) unsigned NOT NULL DEFAULT '0',
  `iRewardMID4` int(10) unsigned NOT NULL DEFAULT '0',
  `fProb4` double(10,3) unsigned NOT NULL DEFAULT '0',
  `iRewardMID5` int(10) unsigned NOT NULL DEFAULT '0',
  `fProb5` double(10,3) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iStage`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `stage_ai`;
CREATE TABLE `stage_ai` (
  `iStage` smallint(5) unsigned NOT NULL,
  `iOrder` tinyint(1) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL,
  `iCLv` smallint(5) unsigned NOT NULL,
  `iDiceID` int(10) unsigned NOT NULL,
  `iMID1` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMID2` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMID3` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMID4` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMID5` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMID6` int(10) unsigned NOT NULL DEFAULT '0',
  `iMLv6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iItem1` int(10) unsigned NOT NULL DEFAULT '0',
  `iQty1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iItem2` int(10) unsigned NOT NULL DEFAULT '0',
  `iQty2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iItem3` int(10) unsigned NOT NULL DEFAULT '0',
  `iQty3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iItem4` int(10) unsigned NOT NULL DEFAULT '0',
  `iQty4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iItem5` int(10) unsigned NOT NULL DEFAULT '0',
  `iQty5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iStage`, `iOrder`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `pull_monster`;
CREATE TABLE `pull_monster` (
  `iPullID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iBuyPrice` int(10) unsigned NOT NULL,
  `fProb1` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb2` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb3` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb4` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb5` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb6` double(10,3) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPullID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
  `iItemID` int(10) unsigned NOT NULL,
  `iCategory` smallint(5) unsigned NOT NULL,
  `iSection` smallint(5) unsigned NOT NULL,
  `iPriceType` tinyint(1) unsigned NOT NULL,
  `iBuyPrice` int(10) unsigned NOT NULL,
  `iSellPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `iEvent` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iBufID` int(10) unsigned NOT NULL,
  `iBufValue` int(10) unsigned NOT NULL,
  `sName` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iItemID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `reinforce_monster`;
CREATE TABLE `reinforce_monster` (
  `iRare` tinyint(1) unsigned NOT NULL,
  `fProb1` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb2` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb3` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb4` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb5` double(10,3) unsigned NOT NULL DEFAULT '0',
  `fProb6` double(10,3) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iRare`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `evolve_monster`;
CREATE TABLE `evolve_monster` (
  `iMID` int(10) unsigned NOT NULL,
  `iNeedLv` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iNeedGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaterialType1` tinyint(1) unsigned NOT NULL,
  `iMaterialID1` int(10) unsigned NOT NULL,
  `iMaterialValue1_1` smallint(5) unsigned NOT NULL,
  `iMaterialValue1_2` smallint(5) unsigned NOT NULL,
  `iMaterialType2` tinyint(1) unsigned NOT NULL,
  `iMaterialID2` int(10) unsigned NOT NULL,
  `iMaterialValue2_1` smallint(5) unsigned NOT NULL,
  `iMaterialValue2_2` smallint(5) unsigned NOT NULL,
  `iMaterialType3` tinyint(1) unsigned NOT NULL,
  `iMaterialID3` int(10) unsigned NOT NULL,
  `iMaterialValue3_1` smallint(5) unsigned NOT NULL,
  `iMaterialValue3_2` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iMID`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/* player_default_data */
DROP TABLE IF EXISTS `player_default_info`;
CREATE TABLE `player_default_info` (
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iFreeCash` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iDiceID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_default_character`;
CREATE TABLE `player_default_character` (
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_default_monster`;
CREATE TABLE `player_default_monster` (
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_default_team`;
CREATE TABLE `player_default_team` (
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_default_inventory`;
CREATE TABLE `player_default_inventory` (
  `iItemID` int(10) unsigned NOT NULL,
  `iQty` smallint(5) unsigned NOT NULL DEFAULT '1',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_default_map`;
CREATE TABLE `player_default_map` (
  `iMapID` int(10) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/* end player_default_data */
