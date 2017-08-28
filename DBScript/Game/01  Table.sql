CREATE DATABASE `summon_game`;
USE summon_game;


DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `iUID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `sAccount` char(32) CHARACTER SET utf8 NOT NULL,
  `sNickName` char(32) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `iGender` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `sBirthday` char(10) CHARACTER SET utf8 NOT NULL DEFAULT '0000-00-00',
  `sPhotoUrl` char(20) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `iPlatform` tinyint(1) unsigned NOT NULL,
  `iMarketID` int(10) unsigned NOT NULL DEFAULT '0',
  `sPushID` char(50) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `iPushOpt` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `sRecommendCode` char(20) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tStart` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tEnd` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLogined` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iWorld` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iMasterID` int(10) unsigned NOT NULL DEFAULT '0',
  `iChannelID` int(10) unsigned NOT NULL DEFAULT '0',
  `sToken` char(128) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tExpire` int(10) unsigned NOT NULL DEFAULT '0',
  `sIdentify` CHAR(128) CHARACTER SET utf8 DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`),
  UNIQUE KEY `index_account` (`sAccount`),
  UNIQUE KEY `index_nickname` (`sNickName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

DROP TABLE IF EXISTS `player_00`;
CREATE TABLE `player_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iWin` int(10) unsigned NOT NULL DEFAULT '0',
  `iLose` int(10) unsigned NOT NULL DEFAULT '0',
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iLastStage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iAttend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tLastAttend` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tUsedSysCID` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLeftDelFriend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_01`;
CREATE TABLE `player_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iWin` int(10) unsigned NOT NULL DEFAULT '0',
  `iLose` int(10) unsigned NOT NULL DEFAULT '0',
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iLastStage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iAttend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tLastAttend` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tUsedSysCID` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLeftDelFriend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_02`;
CREATE TABLE `player_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iWin` int(10) unsigned NOT NULL DEFAULT '0',
  `iLose` int(10) unsigned NOT NULL DEFAULT '0',
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iLastStage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iAttend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tLastAttend` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tUsedSysCID` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLeftDelFriend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_03`;
CREATE TABLE `player_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iWin` int(10) unsigned NOT NULL DEFAULT '0',
  `iLose` int(10) unsigned NOT NULL DEFAULT '0',
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iLastStage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iAttend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tLastAttend` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tUsedSysCID` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLeftDelFriend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_04`;
CREATE TABLE `player_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iWin` int(10) unsigned NOT NULL DEFAULT '0',
  `iLose` int(10) unsigned NOT NULL DEFAULT '0',
  `iGold` int(10) unsigned NOT NULL DEFAULT '0',
  `iHeart` int(10) unsigned NOT NULL DEFAULT '0',
  `iKey` int(10) unsigned NOT NULL DEFAULT '0',
  `iMaxMonster` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxInven` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iMaxFriend` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iLeaderCharacter` int(10) unsigned NOT NULL DEFAULT '0',
  `iLeaderMonster` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iLastStage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `iAttend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tLastAttend` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tUsedSysCID` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iLeftDelFriend` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `iTutorial` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_cash_00`;
CREATE TABLE `player_cash_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCashAD` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashIOS` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashFree` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_cash_01`;
CREATE TABLE `player_cash_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCashAD` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashIOS` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashFree` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_cash_02`;
CREATE TABLE `player_cash_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCashAD` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashIOS` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashFree` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_cash_03`;
CREATE TABLE `player_cash_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCashAD` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashIOS` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashFree` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_cash_04`;
CREATE TABLE `player_cash_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCashAD` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashIOS` int(10) unsigned NOT NULL DEFAULT '0',
  `iCashFree` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_character_00`;
CREATE TABLE `player_character_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_character_01`;
CREATE TABLE `player_character_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_character_02`;
CREATE TABLE `player_character_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_character_03`;
CREATE TABLE `player_character_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_character_04`;
CREATE TABLE `player_character_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iCID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iCID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_equip_00`;
CREATE TABLE `player_equip_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_equip_01`;
CREATE TABLE `player_equip_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_equip_02`;
CREATE TABLE `player_equip_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_equip_03`;
CREATE TABLE `player_equip_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_equip_04`;
CREATE TABLE `player_equip_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUnlockCnt` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iItemSlot1` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot2` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot3` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot4` int(10) unsigned NOT NULL DEFAULT '0',
  `iItemSlot5` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_00`;
CREATE TABLE `player_monster_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iUniqueID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_01`;
CREATE TABLE `player_monster_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iUniqueID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_02`;
CREATE TABLE `player_monster_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iUniqueID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_03`;
CREATE TABLE `player_monster_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iUniqueID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_04`;
CREATE TABLE `player_monster_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iUniqueID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL DEFAULT '0',
  `iLevel` smallint(5) unsigned NOT NULL DEFAULT '1',
  `iExp` int(10) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iUniqueID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_team_00`;
CREATE TABLE `player_team_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iTeamNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_team_01`;
CREATE TABLE `player_team_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iTeamNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_team_02`;
CREATE TABLE `player_team_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iTeamNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_team_03`;
CREATE TABLE `player_team_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iTeamNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_team_04`;
CREATE TABLE `player_team_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iTeamNo` tinyint(1) unsigned NOT NULL,
  `iIndex1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex3` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex4` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex5` bigint(20) unsigned NOT NULL DEFAULT '0',
  `iIndex6` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iTeamNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_inventory_00`;
CREATE TABLE `player_inventory_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_inventory_01`;
CREATE TABLE `player_inventory_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_inventory_02`;
CREATE TABLE `player_inventory_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_inventory_03`;
CREATE TABLE `player_inventory_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_inventory_04`;
CREATE TABLE `player_inventory_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_book_00`;
CREATE TABLE `player_monster_book_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_book_01`;
CREATE TABLE `player_monster_book_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_book_02`;
CREATE TABLE `player_monster_book_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_book_03`;
CREATE TABLE `player_monster_book_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_monster_book_04`;
CREATE TABLE `player_monster_book_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iLevel` smallint(5) unsigned NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iMID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_map_00`;
CREATE TABLE `player_map_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_map_01`;
CREATE TABLE `player_map_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_map_02`;
CREATE TABLE `player_map_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_map_03`;
CREATE TABLE `player_map_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_map_04`;
CREATE TABLE `player_map_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_info`;
CREATE TABLE `player_friend_info` (
  `iUID` bigint(20) unsigned NOT NULL,
  `sNickName` char(32) CHARACTER SET utf8 NOT NULL,
  `iCID` int(10) unsigned NOT NULL,
  `iCLv` smallint(5) unsigned NOT NULL,
  `iMID` int(10) unsigned NOT NULL,
  `iMLv` smallint(5) unsigned NOT NULL,
  `sComment` varchar(255) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`),
  UNIQUE KEY `index_nickname` (`sNickName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_00`;
CREATE TABLE `player_friend_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL,
  `iUsed` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tUsedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iSendedGift` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tSendedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iFUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_01`;
CREATE TABLE `player_friend_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL,
  `iUsed` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tUsedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iSendedGift` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tSendedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iFUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_02`;
CREATE TABLE `player_friend_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL,
  `iUsed` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tUsedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iSendedGift` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tSendedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iFUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_03`;
CREATE TABLE `player_friend_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL,
  `iUsed` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tUsedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iSendedGift` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tSendedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iFUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_friend_04`;
CREATE TABLE `player_friend_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL,
  `iUsed` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tUsedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `iSendedGift` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `tSendedTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`, `iFUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_dice_00`;
CREATE TABLE `player_dice_00` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iDefaultDice` int(10) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_dice_01`;
CREATE TABLE `player_dice_01` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iDefaultDice` int(10) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_dice_02`;
CREATE TABLE `player_dice_02` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iDefaultDice` int(10) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_dice_03`;
CREATE TABLE `player_dice_03` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iDefaultDice` int(10) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_dice_04`;
CREATE TABLE `player_dice_04` (
  `iUID` bigint(20) unsigned NOT NULL,
  `iDefaultDice` int(10) unsigned NOT NULL,
  `sData` text CHARACTER SET utf8 NOT NULL,
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_post_00`;
CREATE TABLE `player_post_00` (
  `iPostIdx` bigint(20) unsigned NOT NULL,
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iSendType` tinyint(1) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iPostType` tinyint(1) unsigned NOT NULL,
  `iValue1` int(10) unsigned NOT NULL,
  `iValue2` int(10) unsigned NOT NULL DEFAULT '0',
  `tExpire` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPostIdx`, `iUID`),
  INDEX `index_uid` (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_post_01`;
CREATE TABLE `player_post_01` (
  `iPostIdx` bigint(20) unsigned NOT NULL,
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iSendType` tinyint(1) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iPostType` tinyint(1) unsigned NOT NULL,
  `iValue1` int(10) unsigned NOT NULL,
  `iValue2` int(10) unsigned NOT NULL DEFAULT '0',
  `tExpire` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPostIdx`, `iUID`),
  INDEX `index_uid` (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_post_02`;
CREATE TABLE `player_post_02` (
  `iPostIdx` bigint(20) unsigned NOT NULL,
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iSendType` tinyint(1) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iPostType` tinyint(1) unsigned NOT NULL,
  `iValue1` int(10) unsigned NOT NULL,
  `iValue2` int(10) unsigned NOT NULL DEFAULT '0',
  `tExpire` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPostIdx`, `iUID`),
  INDEX `index_uid` (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_post_03`;
CREATE TABLE `player_post_03` (
  `iPostIdx` bigint(20) unsigned NOT NULL,
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iSendType` tinyint(1) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iPostType` tinyint(1) unsigned NOT NULL,
  `iValue1` int(10) unsigned NOT NULL,
  `iValue2` int(10) unsigned NOT NULL DEFAULT '0',
  `tExpire` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPostIdx`, `iUID`),
  INDEX `index_uid` (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `player_post_04`;
CREATE TABLE `player_post_04` (
  `iPostIdx` bigint(20) unsigned NOT NULL,
  `iUID` bigint(20) unsigned NOT NULL,
  `iFUID` bigint(20) unsigned NOT NULL,
  `iSendType` tinyint(1) unsigned NOT NULL,
  `iStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `iPostType` tinyint(1) unsigned NOT NULL,
  `iValue1` int(10) unsigned NOT NULL,
  `iValue2` int(10) unsigned NOT NULL DEFAULT '0',
  `tExpire` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `tCreated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `tUpdated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`iPostIdx`, `iUID`),
  INDEX `index_uid` (`iUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
