USE summon_game;


DROP FUNCTION IF EXISTS `FN_Split`;
DELIMITER //
CREATE FUNCTION `FN_Split`(
  `_sIn` MEDIUMTEXT, 
  `_iPos` INT, 
  `_sSep` CHAR(1)
) RETURNS TEXT CHARSET utf8
BEGIN
  DECLARE _Token MEDIUMTEXT;

  SELECT REPLACE(SUBSTRING(SUBSTRING_INDEX(_sIn, _sSep, _iPos), CHAR_LENGTH(SUBSTRING_INDEX(_sIn, _sSep, _iPos - 1))+1), _sSep, '')
  INTO _Token;

  RETURN _Token;
END //
DELIMITER ;

/* ----------------------------------------------------------------------------------------------------------------------- */

DROP PROCEDURE IF EXISTS `SP_SelectAccountByWeb`;
DELIMITER //
CREATE PROCEDURE `SP_SelectAccountByWeb`(
  IN `_sAccount` CHAR(32),
  IN `_iPlatform` TINYINT(1),
  IN `_iMarketID` INT,
  IN `_sPushID` CHAR(50),
  IN `_sToken` CHAR(128),
  IN `_tExpire` INT,
  IN `_sIdentify` CHAR(128)
)
BEGIN
  DECLARE uid bigint(20) unsigned;
  DECLARE status tinyint(1) unsigned;
  DECLARE block_start timestamp;
  DECLARE block_end timestamp;

  SET uid = 0;
  SET status = 0;
  SET block_start = '0000-00-00 00:00:00';
  SET block_end = '0000-00-00 00:00:00';

  SELECT iUID, iStatus, tStart, tEnd INTO uid, status, block_start, block_end FROM account WHERE sAccount = _sAccount;
  IF uid > 0 THEN
    UPDATE account SET iPlatform = _iPlatform, iMarketID = _iMarketID, sPushID = _sPushID, sToken = _sToken, tExpire = _tExpire, tUpdated = NOW() WHERE iUID = uid;
    SELECT uid, status, block_start, block_end;
  ELSE
    INSERT INTO account(iUID, sAccount, sNickName, iPlatform, iMarketID, sPushID, sToken, tExpire, sIdentify, tUpdated) VALUES(NULL, _sAccount, _sAccount, _iPlatform, _iMarketID, _sPushID, _sToken, _tExpire, _sIdentify, NOW())
      ON DUPLICATE KEY UPDATE iPlatform = _iPlatform, iMarketID = _iMarketID, sPushID = _sPushID, sToken = _sToken, tExpire = _tExpire, sIdentify = _sIdentify, tUpdated = NOW();
    SELECT LAST_INSERT_ID() AS uid, status, block_start, block_end;
  END IF;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectAccount`;
DELIMITER //
CREATE PROCEDURE `SP_SelectAccount`(
  IN `_iUID` BIGINT UNSIGNED
)
BEGIN
  SELECT sNickName, iGender, sBirthday, sPhotoUrl, iPlatform, iMarketID, sPushID, iPushOpt, sRecommendCode, sToken, tExpire, iLogined, iWorld, iMasterID, iChannelID, iStatus, tStart, tEnd FROM account WHERE iUID = _iUID;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerData`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerData`(
  IN `_iUID` BIGINT(20) UNSIGNED
)
BEGIN
  DECLARE tableNo tinyint(3) zerofill;
  SET tableNo = _iUID % 5;
  CASE tableNo
    WHEN 0 THEN
      BEGIN
        SELECT iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend, iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, UNIX_TIMESTAMP(tLastAttend), UNIX_TIMESTAMP(tUsedSysCID), iLeftDelFriend, iTutorial, UNIX_TIMESTAMP(tUpdated), IF(tCreated = tUpdated, 1, 0) FROM player_00 WHERE iUID = _iUID;
        SELECT iCashAD, iCashIOS, iCashFree FROM player_cash_00 WHERE iUID = _iUID;
        SELECT iCID, iLevel, iExp, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_character_00 WHERE iUID = _iUID;
        SELECT iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_equip_00 WHERE iUID = _iUID;
        SELECT iUniqueID, iMID, iLevel, iExp FROM player_monster_00 WHERE iUID = _iUID;
        SELECT iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6 FROM player_team_00 WHERE iUID = _iUID;
        SELECT sData FROM player_inventory_00 WHERE iUID = _iUID;
        SELECT sData FROM player_map_00 WHERE iUID = _iUID;
        SELECT NULL;
        SELECT a.iFUID, a.iStatus, a.iUsed, a.tUsedTime, a.iSendedGift, a.tSendedTime, b.sNickName, b.iCID, b.iCLv, b.iMID, b.iMLv, b.sComment, UNIX_TIMESTAMP(b.tUpdated) FROM player_friend_00 AS a, player_friend_info AS b WHERE a.iUID = _iUID AND b.iUID = a.iFUID;
        SELECT iDefaultDice, sData FROM player_dice_00 WHERE iUID = _iUID;

        UPDATE player_post_00 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_00 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iStatus = 0 LIMIT 50;
      END;

    WHEN 1 THEN
      BEGIN
        SELECT iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend, iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, UNIX_TIMESTAMP(tLastAttend), UNIX_TIMESTAMP(tUsedSysCID), iLeftDelFriend, iTutorial, UNIX_TIMESTAMP(tUpdated), IF(tCreated = tUpdated, 1, 0) FROM player_01 WHERE iUID = _iUID;
        SELECT iCashAD, iCashIOS, iCashFree FROM player_cash_01 WHERE iUID = _iUID;
        SELECT iCID, iLevel, iExp, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_character_01 WHERE iUID = _iUID;
        SELECT iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_equip_01 WHERE iUID = _iUID;
        SELECT iUniqueID, iMID, iLevel, iExp FROM player_monster_01 WHERE iUID = _iUID;
        SELECT iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6 FROM player_team_01 WHERE iUID = _iUID;
        SELECT sData FROM player_inventory_01 WHERE iUID = _iUID;
        SELECT sData FROM player_map_01 WHERE iUID = _iUID;
        SELECT NULL;
        SELECT a.iFUID, a.iStatus, a.iUsed, a.tUsedTime, a.iSendedGift, a.tSendedTime, b.sNickName, b.iCID, b.iCLv, b.iMID, b.iMLv, b.sComment, UNIX_TIMESTAMP(b.tUpdated) FROM player_friend_01 AS a, player_friend_info AS b WHERE a.iUID = _iUID AND b.iUID = a.iFUID;
        SELECT iDefaultDice, sData FROM player_dice_01 WHERE iUID = _iUID;

        UPDATE player_post_01 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_01 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iStatus = 0 LIMIT 50;
      END;

    WHEN 2 THEN
      BEGIN
        SELECT iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend, iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, UNIX_TIMESTAMP(tLastAttend), UNIX_TIMESTAMP(tUsedSysCID), iLeftDelFriend, iTutorial, UNIX_TIMESTAMP(tUpdated), IF(tCreated = tUpdated, 1, 0) FROM player_02 WHERE iUID = _iUID;
        SELECT iCashAD, iCashIOS, iCashFree FROM player_cash_02 WHERE iUID = _iUID;
        SELECT iCID, iLevel, iExp, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_character_02 WHERE iUID = _iUID;
        SELECT iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_equip_02 WHERE iUID = _iUID;
        SELECT iUniqueID, iMID, iLevel, iExp FROM player_monster_02 WHERE iUID = _iUID;
        SELECT iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6 FROM player_team_02 WHERE iUID = _iUID;
        SELECT sData FROM player_inventory_02 WHERE iUID = _iUID;
        SELECT sData FROM player_map_02 WHERE iUID = _iUID;
        SELECT NULL;
        SELECT a.iFUID, a.iStatus, a.iUsed, a.tUsedTime, a.iSendedGift, a.tSendedTime, b.sNickName, b.iCID, b.iCLv, b.iMID, b.iMLv, b.sComment, UNIX_TIMESTAMP(b.tUpdated) FROM player_friend_02 AS a, player_friend_info AS b WHERE a.iUID = _iUID AND b.iUID = a.iFUID;
        SELECT iDefaultDice, sData FROM player_dice_02 WHERE iUID = _iUID;

        UPDATE player_post_02 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_02 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iStatus = 0 LIMIT 50;
      END;

    WHEN 3 THEN
      BEGIN
        SELECT iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend, iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, UNIX_TIMESTAMP(tLastAttend), UNIX_TIMESTAMP(tUsedSysCID), iLeftDelFriend, iTutorial, UNIX_TIMESTAMP(tUpdated), IF(tCreated = tUpdated, 1, 0) FROM player_03 WHERE iUID = _iUID;
        SELECT iCashAD, iCashIOS, iCashFree FROM player_cash_03 WHERE iUID = _iUID;
        SELECT iCID, iLevel, iExp, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_character_03 WHERE iUID = _iUID;
        SELECT iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_equip_03 WHERE iUID = _iUID;
        SELECT iUniqueID, iMID, iLevel, iExp FROM player_monster_03 WHERE iUID = _iUID;
        SELECT iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6 FROM player_team_03 WHERE iUID = _iUID;
        SELECT sData FROM player_inventory_03 WHERE iUID = _iUID;
        SELECT sData FROM player_map_03 WHERE iUID = _iUID;
        SELECT NULL;
        SELECT a.iFUID, a.iStatus, a.iUsed, a.tUsedTime, a.iSendedGift, a.tSendedTime, b.sNickName, b.iCID, b.iCLv, b.iMID, b.iMLv, b.sComment, UNIX_TIMESTAMP(b.tUpdated) FROM player_friend_03 AS a, player_friend_info AS b WHERE a.iUID = _iUID AND b.iUID = a.iFUID;
        SELECT iDefaultDice, sData FROM player_dice_03 WHERE iUID = _iUID;

        UPDATE player_post_03 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_03 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iStatus = 0 LIMIT 50;
      END;

    WHEN 4 THEN
      BEGIN
        SELECT iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend, iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, UNIX_TIMESTAMP(tLastAttend), UNIX_TIMESTAMP(tUsedSysCID), iLeftDelFriend, iTutorial, UNIX_TIMESTAMP(tUpdated), IF(tCreated = tUpdated, 1, 0) FROM player_04 WHERE iUID = _iUID;
        SELECT iCashAD, iCashIOS, iCashFree FROM player_cash_04 WHERE iUID = _iUID;
        SELECT iCID, iLevel, iExp, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_character_04 WHERE iUID = _iUID;
        SELECT iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5 FROM player_equip_04 WHERE iUID = _iUID;
        SELECT iUniqueID, iMID, iLevel, iExp FROM player_monster_04 WHERE iUID = _iUID;
        SELECT iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6 FROM player_team_04 WHERE iUID = _iUID;
        SELECT sData FROM player_inventory_04 WHERE iUID = _iUID;
        SELECT sData FROM player_map_04 WHERE iUID = _iUID;
        SELECT NULL;
        SELECT a.iFUID, a.iStatus, a.iUsed, a.tUsedTime, a.iSendedGift, a.tSendedTime, b.sNickName, b.iCID, b.iCLv, b.iMID, b.iMLv, b.sComment, UNIX_TIMESTAMP(b.tUpdated) FROM player_friend_04 AS a, player_friend_info AS b WHERE a.iUID = _iUID AND b.iUID = a.iFUID;
        SELECT iDefaultDice, sData FROM player_dice_04 WHERE iUID = _iUID;

        UPDATE player_post_04 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_04 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iStatus = 0 LIMIT 50;
      END;
    END CASE;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateChannelShutdown`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateChannelShutdown`(
  IN `_iMID` INT UNSIGNED,
  IN `_iCID` INT UNSIGNED
)
BEGIN
  UPDATE account SET iLogined = 0, iMasterID = 0, iChannelID = 0 WHERE iMasterID = _iMID AND iChannelID = _iCID;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerLoginInfo`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerLoginInfo`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iLogined` TINYINT(3) UNSIGNED,
  IN `_iAID` INT(10),
  IN `_iWID` TINYINT(3) UNSIGNED,
  IN `_iMID` INT(10),
  IN `_iCID` INT(10)
)
BEGIN
  DECLARE status SMALLINT(5) UNSIGNED;
  SET status = 0;

  UPDATE account SET iLogined = _iLogined, iWorld = _iWID, iMasterID = _iMID, iChannelID = _iCID, tUpdated = NOW() WHERE iUID = _iUID;

  SELECT iStatus INTO status FROM account WHERE iUID = _iUID;
  IF _iLogined = 0 AND status = 0 THEN
    UPDATE account SET sNickName = sAccount WHERE iUID = _iUID;
  END IF;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerData`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerData`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sPlayerInfo` TEXT,
  IN `_sPlayerCash` TEXT,
  IN `_sPlayerCharacter` TEXT,
  IN `_sPlayerEquip` TEXT,
  IN `_sPlayerMonster` TEXT,
  IN `_sPlayerTeam` TEXT,
  IN `_sPlayerInv` TEXT,
  IN `_sPlayerMap` TEXT,
  IN `_sFriendInfo` TEXT,
  IN `_sFriend` TEXT,
  IN `_sPlayerDice` TEXT,
  IN `_sPlayerPost` TEXT
)
BEGIN
  DECLARE error INT(10) DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET error = -1;

  START TRANSACTION;

  CALL SP_UpdatePlayerInfo(_iUID, _sPlayerInfo);
  CALL SP_UpdatePlayerCash(_iUID, _sPlayerCash);
  CALL SP_UpdatePlayerCharacter(_iUID, _sPlayerCharacter);
  CALL SP_UpdatePlayerEquip(_iUID, _sPlayerEquip);
  CALL SP_UpdatePlayerMonster(_iUID, _sPlayerMonster);
  CALL SP_UpdatePlayerTeam(_iUID, _sPlayerTeam);
  CALL SP_UpdatePlayerInventory(_iUID, _sPlayerInv);
  CALL SP_UpdatePlayerMap(_iUID, _sPlayerMap);
  CALL SP_UpdatePlayerFriendInfo(_iUID, _sFriendInfo);
  CALL SP_UpdatePlayerFriend(_iUID, _sFriend);
  CALL SP_UpdatePlayerDice(_iUID, _sPlayerDice);
  CALL SP_UpdatePlayerPost(_iUID, _sPlayerPost);

  IF error < 0 THEN
    SELECT 1 AS result, _iUID;
    ROLLBACK;
  ELSE
    SELECT 0 AS result, _iUID;
    COMMIT;
  END IF;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerInfo`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerInfo`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE win INT(10) UNSIGNED;
  DECLARE lose INT(10) UNSIGNED;
  DECLARE gold INT(10) UNSIGNED;
  DECLARE heart INT(10) UNSIGNED;
  DECLARE ikey INT(10) UNSIGNED;
  DECLARE max_monster SMALLINT(5) UNSIGNED;
  DECLARE max_inv SMALLINT(5) UNSIGNED;
  DECLARE max_friend SMALLINT(5) UNSIGNED;
  DECLARE leader_character INT(10) UNSIGNED;
  DECLARE leader_monster BIGINT(20) UNSIGNED;
  DECLARE stage SMALLINT(5) UNSIGNED;
  DECLARE attend SMALLINT(5) UNSIGNED;
  DECLARE attend_time INT(10) UNSIGNED;
  DECLARE used_sys_time INT(10) UNSIGNED;
  DECLARE left_del_friend TINYINT(3) UNSIGNED;
  DECLARE tutorial SMALLINT(5) UNSIGNED;

  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ',') INTO win;
    SELECT FN_Split(_sData, 2, ',') INTO lose;
    SELECT FN_Split(_sData, 3, ',') INTO gold;
    SELECT FN_Split(_sData, 4, ',') INTO heart;
    SELECT FN_Split(_sData, 5, ',') INTO ikey;
    SELECT FN_Split(_sData, 6, ',') INTO max_monster;
    SELECT FN_Split(_sData, 7, ',') INTO max_inv;
    SELECT FN_Split(_sData, 8, ',') INTO max_friend;
    SELECT FN_Split(_sData, 9, ',') INTO leader_character;
    SELECT FN_Split(_sData, 10, ',') INTO leader_monster;
	SELECT FN_Split(_sData, 11, ',') INTO stage;
    SELECT FN_Split(_sData, 12, ',') INTO attend;
    SELECT FN_Split(_sData, 13, ',') INTO attend_time;
    SELECT FN_Split(_sData, 14, ',') INTO used_sys_time;
    SELECT FN_Split(_sData, 15, ',') INTO left_del_friend;
    SELECT FN_Split(_sData, 16, ',') INTO tutorial;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_00(
          iUID, iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend,
          iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, tLastAttend, tUsedSysCID, iLeftDelFriend, iTutorial, tUpdated) VALUES(
          _iUID, win, lose, gold, heart, ikey, max_monster, max_inv, max_friend,
          leader_character, leader_monster, stage, attend, FROM_UNIXTIME(attend_time), FROM_UNIXTIME(used_sys_time), left_del_friend, tutorial, NOW()) ON DUPLICATE KEY UPDATE 
          iWin = win, iLose = lose, iGold = gold, iHeart = heart, iKey = ikey, 
          iMaxMonster = max_monster, iMaxInven = max_inv, iMaxFriend = max_friend, 
          iLeaderCharacter = leader_character, iLeaderMonster = leader_monster, iLastStage = stage, 
          iAttend = attend, tLastAttend = FROM_UNIXTIME(attend_time), tUsedSysCID = FROM_UNIXTIME(used_sys_time), 
          iLeftDelFriend = left_del_friend, iTutorial = tutorial, tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_01(
          iUID, iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend,
          iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, tLastAttend, tUsedSysCID, iLeftDelFriend, iTutorial, tUpdated) VALUES(
          _iUID, win, lose, gold, heart, ikey, max_monster, max_inv, max_friend,
          leader_character, leader_monster, stage, attend, FROM_UNIXTIME(attend_time), FROM_UNIXTIME(used_sys_time), left_del_friend, tutorial, NOW()) ON DUPLICATE KEY UPDATE 
          iWin = win, iLose = lose, iGold = gold, iHeart = heart, iKey = ikey, 
          iMaxMonster = max_monster, iMaxInven = max_inv, iMaxFriend = max_friend, 
          iLeaderCharacter = leader_character, iLeaderMonster = leader_monster, iLastStage = stage, 
          iAttend = attend, tLastAttend = FROM_UNIXTIME(attend_time), tUsedSysCID = FROM_UNIXTIME(used_sys_time), 
          iLeftDelFriend = left_del_friend, iTutorial = tutorial, tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_02(
          iUID, iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend,
          iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, tLastAttend, tUsedSysCID, iLeftDelFriend, iTutorial, tUpdated) VALUES(
          _iUID, win, lose, gold, heart, ikey, max_monster, max_inv, max_friend,
          leader_character, leader_monster, stage, attend, FROM_UNIXTIME(attend_time), FROM_UNIXTIME(used_sys_time), left_del_friend, tutorial, NOW()) ON DUPLICATE KEY UPDATE 
          iWin = win, iLose = lose, iGold = gold, iHeart = heart, iKey = ikey, 
          iMaxMonster = max_monster, iMaxInven = max_inv, iMaxFriend = max_friend, 
          iLeaderCharacter = leader_character, iLeaderMonster = leader_monster, iLastStage = stage, 
          iAttend = attend, tLastAttend = FROM_UNIXTIME(attend_time), tUsedSysCID = FROM_UNIXTIME(used_sys_time), 
          iLeftDelFriend = left_del_friend, iTutorial = tutorial, tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_03(
          iUID, iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend,
          iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, tLastAttend, tUsedSysCID, iLeftDelFriend, iTutorial, tUpdated) VALUES(
          _iUID, win, lose, gold, heart, ikey, max_monster, max_inv, max_friend,
          leader_character, leader_monster, stage, attend, FROM_UNIXTIME(attend_time), FROM_UNIXTIME(used_sys_time), left_del_friend, tutorial, NOW()) ON DUPLICATE KEY UPDATE 
          iWin = win, iLose = lose, iGold = gold, iHeart = heart, iKey = ikey, 
          iMaxMonster = max_monster, iMaxInven = max_inv, iMaxFriend = max_friend, 
          iLeaderCharacter = leader_character, iLeaderMonster = leader_monster, iLastStage = stage, 
          iAttend = attend, tLastAttend = FROM_UNIXTIME(attend_time), tUsedSysCID = FROM_UNIXTIME(used_sys_time), 
          iLeftDelFriend = left_del_friend, iTutorial = tutorial, tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_04(
          iUID, iWin, iLose, iGold, iHeart, iKey, iMaxMonster, iMaxInven, iMaxFriend,
          iLeaderCharacter, iLeaderMonster, iLastStage, iAttend, tLastAttend, tUsedSysCID, iLeftDelFriend, iTutorial, tUpdated) VALUES(
          _iUID, win, lose, gold, heart, ikey, max_monster, max_inv, max_friend,
          leader_character, leader_monster, stage, attend, FROM_UNIXTIME(attend_time), FROM_UNIXTIME(used_sys_time), left_del_friend, tutorial, NOW()) ON DUPLICATE KEY UPDATE 
          iWin = win, iLose = lose, iGold = gold, iHeart = heart, iKey = ikey, 
          iMaxMonster = max_monster, iMaxInven = max_inv, iMaxFriend = max_friend, 
          iLeaderCharacter = leader_character, iLeaderMonster = leader_monster, iLastStage = stage, 
          iAttend = attend, tLastAttend = FROM_UNIXTIME(attend_time), tUsedSysCID = FROM_UNIXTIME(used_sys_time), 
          iLeftDelFriend = left_del_friend, iTutorial = tutorial, tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerCash`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerCash`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE ad INT(10) UNSIGNED;
  DECLARE ios INT(10) UNSIGNED;
  DECLARE free INT(10) UNSIGNED;

  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ',') INTO ad;
    SELECT FN_Split(_sData, 2, ',') INTO ios;
    SELECT FN_Split(_sData, 3, ',') INTO free;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_cash_00(
          iUID, iCashAD, iCashIOS, iCashFree, tUpdated) VALUES(
          _iUID, ad, ios, free, NOW()) ON DUPLICATE KEY UPDATE 
          iCashAd = ad, iCashIOS = ios, iCashFree = free, tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_cash_01(
          iUID, iCashAD, iCashIOS, iCashFree, tUpdated) VALUES(
          _iUID, ad, ios, free, NOW()) ON DUPLICATE KEY UPDATE 
          iCashAd = ad, iCashIOS = ios, iCashFree = free, tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_cash_02(
          iUID, iCashAD, iCashIOS, iCashFree, tUpdated) VALUES(
          _iUID, ad, ios, free, NOW()) ON DUPLICATE KEY UPDATE 
          iCashAd = ad, iCashIOS = ios, iCashFree = free, tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_cash_03(
          iUID, iCashAD, iCashIOS, iCashFree, tUpdated) VALUES(
          _iUID, ad, ios, free, NOW()) ON DUPLICATE KEY UPDATE 
          iCashAd = ad, iCashIOS = ios, iCashFree = free, tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_cash_04(
          iUID, iCashAD, iCashIOS, iCashFree, tUpdated) VALUES(
          _iUID, ad, ios, free, NOW()) ON DUPLICATE KEY UPDATE 
          iCashAd = ad, iCashIOS = ios, iCashFree = free, tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerCharacter`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerCharacter`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE cnt INT(10) UNSIGNED;
  DECLARE totalcnt INT(10) UNSIGNED;
  DECLARE tokens TEXT;
  DECLARE t TEXT;

  DECLARE id INT(10) UNSIGNED;
  DECLARE lv SMALLINT(5) UNSIGNED;
  DECLARE ex INT(10) UNSIGNED;
  DECLARE unlockcnt TINYINT(3) UNSIGNED;
  DECLARE item1 INT(10) UNSIGNED;
  DECLARE item2 INT(10) UNSIGNED;
  DECLARE item3 INT(10) UNSIGNED;
  DECLARE item4 INT(10) UNSIGNED;
  DECLARE item5 INT(10) UNSIGNED;

  SET tableNo = _iUID % 5;
  SET cnt = 1;
  SET totalcnt = 0;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ':') INTO totalcnt;
    SELECT FN_Split(_sData, 2, ':') INTO tokens;
  END IF;

  WHILE cnt <= totalcnt DO
    SELECT FN_Split(tokens, cnt, '|') INTO t;

    SELECT FN_Split(t, 1, ',') INTO id;
    SELECT FN_Split(t, 2, ',') INTO lv;
    SELECT FN_Split(t, 3, ',') INTO ex;
    SELECT FN_Split(t, 4, ',') INTO unlockcnt;
    SELECT FN_Split(t, 5, ',') INTO item1;
    SELECT FN_Split(t, 6, ',') INTO item2;
    SELECT FN_Split(t, 7, ',') INTO item3;
    SELECT FN_Split(t, 8, ',') INTO item4;
    SELECT FN_Split(t, 9, ',') INTO item5;

    CASE tableNo
      WHEN 0 THEN
        BEGIN
          INSERT IGNORE INTO player_character_00(
            iUID, iCID, iLevel, iExp, iUnlockCnt, 
            iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
            _iUID, id, lv, ex, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
            iLevel = lv, iExp = ex, iUnlockCnt = unlockcnt, 
            iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
            tUpdated = NOW();
        END;

      WHEN 1 THEN
        BEGIN
          INSERT IGNORE INTO player_character_01(
            iUID, iCID, iLevel, iExp, iUnlockCnt, 
            iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
            _iUID, id, lv, ex, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
            iLevel = lv, iExp = ex, iUnlockCnt = unlockcnt, 
            iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
            tUpdated = NOW();
        END;

      WHEN 2 THEN
        BEGIN
          INSERT IGNORE INTO player_character_02(
            iUID, iCID, iLevel, iExp, iUnlockCnt, 
            iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
            _iUID, id, lv, ex, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
            iLevel = lv, iExp = ex, iUnlockCnt = unlockcnt, 
            iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
            tUpdated = NOW();
        END;

      WHEN 3 THEN
        BEGIN
          INSERT IGNORE INTO player_character_03(
            iUID, iCID, iLevel, iExp, iUnlockCnt, 
            iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
            _iUID, id, lv, ex, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
            iLevel = lv, iExp = ex, iUnlockCnt = unlockcnt, 
            iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
            tUpdated = NOW();
        END;

      WHEN 4 THEN
        BEGIN
          INSERT IGNORE INTO player_character_04(
            iUID, iCID, iLevel, iExp, iUnlockCnt, 
            iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
            _iUID, id, lv, ex, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
            iLevel = lv, iExp = ex, iUnlockCnt = unlockcnt, 
            iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
            tUpdated = NOW();
        END;
    END CASE;

    SET cnt = cnt + 1;
  END WHILE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerEquip`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerEquip`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE unlockcnt TINYINT(3) UNSIGNED;
  DECLARE item1 INT(10) UNSIGNED;
  DECLARE item2 INT(10) UNSIGNED;
  DECLARE item3 INT(10) UNSIGNED;
  DECLARE item4 INT(10) UNSIGNED;
  DECLARE item5 INT(10) UNSIGNED;

  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ',') INTO unlockcnt;
    SELECT FN_Split(_sData, 2, ',') INTO item1;
    SELECT FN_Split(_sData, 3, ',') INTO item2;
    SELECT FN_Split(_sData, 4, ',') INTO item3;
    SELECT FN_Split(_sData, 5, ',') INTO item4;
    SELECT FN_Split(_sData, 6, ',') INTO item5;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_equip_00(
          iUID, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
          _iUID, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
          iUnlockCnt = unlockcnt, 
          iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
          tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_equip_01(
          iUID, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
          _iUID, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
          iUnlockCnt = unlockcnt, 
          iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
          tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_equip_02(
          iUID, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
          _iUID, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
          iUnlockCnt = unlockcnt, 
          iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
          tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_equip_03(
          iUID, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
          _iUID, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
          iUnlockCnt = unlockcnt, 
          iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
          tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_equip_04(
          iUID, iUnlockCnt, iItemSlot1, iItemSlot2, iItemSlot3, iItemSlot4, iItemSlot5, tUpdated) VALUES(
          _iUID, unlockcnt, item1, item2, item3, item4, item5, NOW()) ON DUPLICATE KEY UPDATE 
          iUnlockCnt = unlockcnt, 
          iItemSlot1 = item1, iItemSlot2 = item2, iItemSlot3 = item3, iItemSlot4 = item4, iItemSlot5 = item5, 
          tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerMonster`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerMonster`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE cnt INT(10) UNSIGNED;
  DECLARE totalcnt INT(10) UNSIGNED;
  DECLARE tokens TEXT;
  DECLARE t TEXT;
  
  DECLARE uniq BIGINT(10) UNSIGNED;
  DECLARE id INT(10) UNSIGNED;
  DECLARE lv SMALLINT(5) UNSIGNED;
  DECLARE ex INT(10) UNSIGNED;

  SET tableNo = _iUID % 5;
  SET cnt = 1;
  SET totalcnt = 0;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ':') INTO totalcnt;
    SELECT FN_Split(_sData, 2, ':') INTO tokens;
  END IF;

  WHILE cnt <= totalcnt DO
    SELECT FN_Split(tokens, cnt, '|') INTO t;

    SELECT FN_Split(t, 1, ',') INTO uniq;
    SELECT FN_Split(t, 2, ',') INTO id;
    SELECT FN_Split(t, 3, ',') INTO lv;
    SELECT FN_Split(t, 4, ',') INTO ex;

    CASE tableNo
      WHEN 0 THEN
        BEGIN
          INSERT IGNORE INTO player_monster_00(
            iUID, iUniqueID, iMID, iLevel, iExp, tUpdated) VALUES(
            _iUID, uniq, id, lv, ex, NOW()) ON DUPLICATE KEY UPDATE 
            iMID = id, iLevel = lv, iExp = ex, tUpdated = NOW();
        END;

      WHEN 1 THEN
        BEGIN
          INSERT IGNORE INTO player_monster_01(
            iUID, iUniqueID, iMID, iLevel, iExp, tUpdated) VALUES(
            _iUID, uniq, id, lv, ex, NOW()) ON DUPLICATE KEY UPDATE 
            iMID = id, iLevel = lv, iExp = ex, tUpdated = NOW();
        END;

      WHEN 2 THEN
        BEGIN
          INSERT IGNORE INTO player_monster_02(
            iUID, iUniqueID, iMID, iLevel, iExp, tUpdated) VALUES(
            _iUID, uniq, id, lv, ex, NOW()) ON DUPLICATE KEY UPDATE 
            iMID = id, iLevel = lv, iExp = ex, tUpdated = NOW();
        END;

      WHEN 3 THEN
        BEGIN
          INSERT IGNORE INTO player_monster_03(
            iUID, iUniqueID, iMID, iLevel, iExp, tUpdated) VALUES(
            _iUID, uniq, id, lv, ex, NOW()) ON DUPLICATE KEY UPDATE 
            iMID = id, iLevel = lv, iExp = ex, tUpdated = NOW();
        END;

      WHEN 4 THEN
        BEGIN
          INSERT IGNORE INTO player_monster_04(
            iUID, iUniqueID, iMID, iLevel, iExp, tUpdated) VALUES(
            _iUID, uniq, id, lv, ex, NOW()) ON DUPLICATE KEY UPDATE 
            iMID = id, iLevel = lv, iExp = ex, tUpdated = NOW();
        END;
    END CASE;

    SET cnt = cnt + 1;
  END WHILE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerTeam`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerTeam`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE cnt INT(10) UNSIGNED;
  DECLARE totalcnt INT(10) UNSIGNED;
  DECLARE tokens TEXT;
  DECLARE t TEXT;
  
  DECLARE teamNo TINYINT(3) UNSIGNED;
  DECLARE team1 BIGINT(20) UNSIGNED;
  DECLARE team2 BIGINT(20) UNSIGNED;
  DECLARE team3 BIGINT(20) UNSIGNED;
  DECLARE team4 BIGINT(20) UNSIGNED;
  DECLARE team5 BIGINT(20) UNSIGNED;
  DECLARE team6 BIGINT(20) UNSIGNED;

  SET tableNo = _iUID % 5;
  SET cnt = 1;
  SET totalcnt = 0;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ':') INTO totalcnt;
    SELECT FN_Split(_sData, 2, ':') INTO tokens;
  END IF;

  WHILE cnt <= totalcnt DO
    SELECT FN_Split(tokens, cnt, '|') INTO t;

    SELECT FN_Split(t, 1, ',') INTO teamNo;
    SELECT FN_Split(t, 2, ',') INTO team1;
    SELECT FN_Split(t, 3, ',') INTO team2;
    SELECT FN_Split(t, 4, ',') INTO team3;
    SELECT FN_Split(t, 5, ',') INTO team4;
    SELECT FN_Split(t, 6, ',') INTO team5;
    SELECT FN_Split(t, 7, ',') INTO team6;

    CASE tableNo
      WHEN 0 THEN
        BEGIN
          INSERT IGNORE INTO player_team_00(
            iUID, iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6, tUpdated) VALUES(
            _iUID, teamNo, team1, team2, team3, team4, team5, team6, NOW()) ON DUPLICATE KEY UPDATE 
            iIndex1 = team1, iIndex2 = team2, iIndex3 = team3, 
            iIndex4 = team4, iIndex5 = team5, iIndex6 = team6, tUpdated = NOW();
        END;

      WHEN 1 THEN
        BEGIN
          INSERT IGNORE INTO player_team_01(
            iUID, iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6, tUpdated) VALUES(
            _iUID, teamNo, team1, team2, team3, team4, team5, team6, NOW()) ON DUPLICATE KEY UPDATE 
            iIndex1 = team1, iIndex2 = team2, iIndex3 = team3, 
            iIndex4 = team4, iIndex5 = team5, iIndex6 = team6, tUpdated = NOW();
        END;

      WHEN 2 THEN
        BEGIN
          INSERT IGNORE INTO player_team_02(
            iUID, iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6, tUpdated) VALUES(
            _iUID, teamNo, team1, team2, team3, team4, team5, team6, NOW()) ON DUPLICATE KEY UPDATE 
            iIndex1 = team1, iIndex2 = team2, iIndex3 = team3, 
            iIndex4 = team4, iIndex5 = team5, iIndex6 = team6, tUpdated = NOW();
        END;

      WHEN 3 THEN
        BEGIN
          INSERT IGNORE INTO player_team_03(
            iUID, iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6, tUpdated) VALUES(
            _iUID, teamNo, team1, team2, team3, team4, team5, team6, NOW()) ON DUPLICATE KEY UPDATE 
            iIndex1 = team1, iIndex2 = team2, iIndex3 = team3, 
            iIndex4 = team4, iIndex5 = team5, iIndex6 = team6, tUpdated = NOW();
        END;

      WHEN 4 THEN
        BEGIN
          INSERT IGNORE INTO player_team_04(
            iUID, iTeamNo, iIndex1, iIndex2, iIndex3, iIndex4, iIndex5, iIndex6, tUpdated) VALUES(
            _iUID, teamNo, team1, team2, team3, team4, team5, team6, NOW()) ON DUPLICATE KEY UPDATE 
            iIndex1 = team1, iIndex2 = team2, iIndex3 = team3, 
            iIndex4 = team4, iIndex5 = team5, iIndex6 = team6, tUpdated = NOW();
        END;
    END CASE;

    SET cnt = cnt + 1;
  END WHILE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerInventory`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerInventory`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;
  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_inventory_00(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_inventory_01(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_inventory_02(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_inventory_03(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_inventory_04(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerMap`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerMap`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;
  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_map_00(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_map_01(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_map_02(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_map_03(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_map_04(
          iUID, sData, tUpdated) VALUES(
          _iUID, _sData, NOW()) ON DUPLICATE KEY UPDATE 
          sData = _sData, tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerFriendInfo`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerFriendInfo`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE nickname CHAR(32);
  DECLARE cid INT(10) UNSIGNED;
  DECLARE clv SMALLINT(5) UNSIGNED;
  DECLARE monsterid INT(10) UNSIGNED;
  DECLARE monsterlv SMALLINT(5) UNSIGNED;
  DECLARE comments VARCHAR(255);

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ',') INTO nickname;
    SELECT FN_Split(_sData, 2, ',') INTO cid;
    SELECT FN_Split(_sData, 3, ',') INTO clv;
    SELECT FN_Split(_sData, 4, ',') INTO monsterid;
    SELECT FN_Split(_sData, 5, ',') INTO monsterlv;
    SELECT FN_Split(_sData, 6, ',') INTO comments;
  END IF;

  INSERT IGNORE INTO player_friend_info(
    iUID, sNickName, iCID, iCLv, iMID, iMLv, sComment, tUpdated) VALUES(
    _iUID, nickname, cid, clv, monsterid, monsterlv, comments, NOW()) ON DUPLICATE KEY UPDATE 
    sNickName = nickname, iCID = cid, iCLv = clv, iMID = monsterid, iMLv = monsterlv, sComment = comments, tUpdated = NOW();
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerFriend`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerFriend`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE cnt INT(10) UNSIGNED;
  DECLARE totalcnt INT(10) UNSIGNED;
  DECLARE tokens TEXT;
  DECLARE t TEXT;

  DECLARE fuid BIGINT(20) UNSIGNED;
  DECLARE used TINYINT(3) UNSIGNED;
  DECLARE used_time INT(10) UNSIGNED;
  DECLARE sended TINYINT(3) UNSIGNED;
  DECLARE sended_time INT(10) UNSIGNED;

  SET tableNo = _iUID % 5;
  SET cnt = 1;
  SET totalcnt = 0;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ':') INTO totalcnt;
    SELECT FN_Split(_sData, 2, ':') INTO tokens;
  END IF;

  WHILE cnt <= totalcnt DO
    SELECT FN_Split(tokens, cnt, '|') INTO t;

    SELECT FN_Split(t, 1, ',') INTO fuid;
    SELECT FN_Split(t, 2, ',') INTO used;
    SELECT FN_Split(t, 3, ',') INTO used_time;
    SELECT FN_Split(t, 4, ',') INTO sended;
    SELECT FN_Split(t, 5, ',') INTO sended_time;

	CASE tableNo
      WHEN 0 THEN
        BEGIN
          UPDATE player_friend_00 SET iUsed = used, tUsedTime = FROM_UNIXTIME(used_time), iSendedGift = sended, tSendedTime = FROM_UNIXTIME(sended_time), tUpdated = NOW()
          WHERE iUID = _iUID AND iFUID = fuid;
        END;

      WHEN 1 THEN
        BEGIN
          UPDATE player_friend_01 SET iUsed = used, tUsedTime = FROM_UNIXTIME(used_time), iSendedGift = sended, tSendedTime = FROM_UNIXTIME(sended_time), tUpdated = NOW()
          WHERE iUID = _iUID AND iFUID = fuid;
        END;

      WHEN 2 THEN
        BEGIN
          UPDATE player_friend_02 SET iUsed = used, tUsedTime = FROM_UNIXTIME(used_time), iSendedGift = sended, tSendedTime = FROM_UNIXTIME(sended_time), tUpdated = NOW()
          WHERE iUID = _iUID AND iFUID = fuid;
        END;

      WHEN 3 THEN
        BEGIN
          UPDATE player_friend_03 SET iUsed = used, tUsedTime = FROM_UNIXTIME(used_time), iSendedGift = sended, tSendedTime = FROM_UNIXTIME(sended_time), tUpdated = NOW()
          WHERE iUID = _iUID AND iFUID = fuid;
        END;

      WHEN 4 THEN
        BEGIN
          UPDATE player_friend_04 SET iUsed = used, tUsedTime = FROM_UNIXTIME(used_time), iSendedGift = sended, tSendedTime = FROM_UNIXTIME(sended_time), tUpdated = NOW()
          WHERE iUID = _iUID AND iFUID = fuid;
        END;
    END CASE;

    SET cnt = cnt + 1;
  END WHILE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerDice`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerDice`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE defaultdice INT(10) UNSIGNED;
  DECLARE dicedata TEXT;

  SET tableNo = _iUID % 5;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, '/') INTO defaultdice;
    SELECT FN_Split(_sData, 2, '/') INTO dicedata;
  END IF;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT IGNORE INTO player_dice_00(
          iUID, iDefaultDice, sData, tUpdated) VALUES(
          _iUID, defaultdice, dicedata, NOW()) ON DUPLICATE KEY UPDATE 
          iDefaultDice = defaultdice, sData = dicedata, tUpdated = NOW();
      END;

    WHEN 1 THEN
      BEGIN
        INSERT IGNORE INTO player_dice_01(
          iUID, iDefaultDice, sData, tUpdated) VALUES(
          _iUID, defaultdice, dicedata, NOW()) ON DUPLICATE KEY UPDATE 
          iDefaultDice = defaultdice, sData = dicedata, tUpdated = NOW();
      END;

    WHEN 2 THEN
      BEGIN
        INSERT IGNORE INTO player_dice_02(
          iUID, iDefaultDice, sData, tUpdated) VALUES(
          _iUID, defaultdice, dicedata, NOW()) ON DUPLICATE KEY UPDATE 
          iDefaultDice = defaultdice, sData = dicedata, tUpdated = NOW();
      END;

    WHEN 3 THEN
      BEGIN
        INSERT IGNORE INTO player_dice_03(
          iUID, iDefaultDice, sData, tUpdated) VALUES(
          _iUID, defaultdice, dicedata, NOW()) ON DUPLICATE KEY UPDATE 
          iDefaultDice = defaultdice, sData = dicedata, tUpdated = NOW();
      END;

    WHEN 4 THEN
      BEGIN
        INSERT IGNORE INTO player_dice_04(
          iUID, iDefaultDice, sData, tUpdated) VALUES(
          _iUID, defaultdice, dicedata, NOW()) ON DUPLICATE KEY UPDATE 
          iDefaultDice = defaultdice, sData = dicedata, tUpdated = NOW();
      END;
  END CASE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerPost`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerPost`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sData` TEXT
)
BEGIN
  `RUN`:
  BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;

  DECLARE cnt INT(10) UNSIGNED;
  DECLARE totalcnt INT(10) UNSIGNED;
  DECLARE tokens TEXT;
  DECLARE t TEXT;

  DECLARE idx BIGINT(20) UNSIGNED;
  DECLARE status TINYINT(3) UNSIGNED;

  SET tableNo = _iUID % 5;
  SET cnt = 1;
  SET totalcnt = 0;

  IF _sData = '' OR _sData = 'null' THEN
    LEAVE `RUN`;
  ELSE
    SELECT FN_Split(_sData, 1, ':') INTO totalcnt;
    SELECT FN_Split(_sData, 2, ':') INTO tokens;
  END IF;

  WHILE cnt <= totalcnt DO
    SELECT FN_Split(tokens, cnt, '|') INTO t;

    SELECT FN_Split(t, 1, ',') INTO idx;
    SELECT FN_Split(t, 2, ',') INTO status;

	CASE tableNo
      WHEN 0 THEN
        BEGIN
          UPDATE player_post_00 SET iStatus = status, tUpdated = NOW() WHERE iPostIdx = idx AND iUID = _iUID;
        END;

      WHEN 1 THEN
        BEGIN
          UPDATE player_post_01 SET iStatus = status, tUpdated = NOW() WHERE iPostIdx = idx AND iUID = _iUID;
        END;

      WHEN 2 THEN
        BEGIN
          UPDATE player_post_02 SET iStatus = status, tUpdated = NOW() WHERE iPostIdx = idx AND iUID = _iUID;
        END;

      WHEN 3 THEN
        BEGIN
          UPDATE player_post_03 SET iStatus = status, tUpdated = NOW() WHERE iPostIdx = idx AND iUID = _iUID;
        END;

      WHEN 4 THEN
        BEGIN
          UPDATE player_post_04 SET iStatus = status, tUpdated = NOW() WHERE iPostIdx = idx AND iUID = _iUID;
        END;
    END CASE;

    SET cnt = cnt + 1;
  END WHILE;
  END `RUN`;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectNickname`;
DELIMITER //
CREATE PROCEDURE `SP_SelectNickname`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_sNickname` CHAR(32)
)
BEGIN
  DECLARE duplicate INT(10) UNSIGNED;
  SET duplicate = 0;

  SELECT COUNT(*) INTO duplicate FROM account WHERE sNickName = _sNickname;

  IF duplicate = 0 THEN
    UPDATE account SET sNickName = _sNickname WHERE iUID = _iUID;
  END IF;

  SELECT duplicate;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdatePlayerNewAccount`;
DELIMITER //
CREATE PROCEDURE `SP_UpdatePlayerNewAccount`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iCID` INT(10) UNSIGNED
)
BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;
  SET tableNo = _iUID % 5;

  UPDATE account SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        INSERT INTO player_character_00(iUID, iCID, tUpdated) VALUES(_iUID, _iCID, NOW());
      END;

	WHEN 1 THEN
      BEGIN
        INSERT INTO player_character_01(iUID, iCID, tUpdated) VALUES(_iUID, _iCID, NOW());
      END;

	WHEN 2 THEN
      BEGIN
        INSERT INTO player_character_02(iUID, iCID, tUpdated) VALUES(_iUID, _iCID, NOW());
      END;

	WHEN 3 THEN
      BEGIN
        INSERT INTO player_character_03(iUID, iCID, tUpdated) VALUES(_iUID, _iCID, NOW());
      END;

	WHEN 4 THEN
      BEGIN
        INSERT INTO player_character_04(iUID, iCID, tUpdated) VALUES(_iUID, _iCID, NOW());
      END;
  END CASE;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectRecommendFriend`;
DELIMITER //
CREATE PROCEDURE `SP_SelectRecommendFriend`(
  IN `_iMinSystemUID` BIGINT(20) UNSIGNED,
  IN `_iMinRecommendUID` BIGINT(20) UNSIGNED,
  IN `_iCount` INT(10),
  IN `_iLoadSystem` TINYINT(1)
)
BEGIN
  IF _iLoadSystem = 1 THEN
    SELECT iUID, sNickName, iCID, iCLv, iMID, iMLv, sComment, UNIX_TIMESTAMP(tUpdated) FROM player_friend_info WHERE iUID >= _iMinSystemUID AND iUID < _iMinRecommendUID;
    SELECT iUID, sNickName, iCID, iCLv, iMID, iMLv, sComment, UNIX_TIMESTAMP(tUpdated) FROM player_friend_info WHERE iUID >= _iMinRecommendUID ORDER BY RAND() LIMIT _iCount;
  ELSE
    SELECT iUID, sNickName, iCID, iCLv, iMID, iMLv, sComment, UNIX_TIMESTAMP(tUpdated) FROM player_friend_info WHERE iUID >= _iMinRecommendUID ORDER BY RAND() LIMIT _iCount;
  END IF;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectFriendSearch`;
DELIMITER //
CREATE PROCEDURE `SP_SelectFriendSearch`(
  IN `_sNickName` CHAR(32)
)
BEGIN
  SELECT COUNT(*), iUID, sNickName, iCID, iCLv, iMID, iMLv, sComment, UNIX_TIMESTAMP(tUpdated) FROM player_friend_info WHERE sNickName = _sNickName;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateFriendStatus`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateFriendStatus`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iFUID` BIGINT(20) UNSIGNED,
  IN `_iAct` TINYINT(1) UNSIGNED
)
BEGIN
  CASE _iAct
    WHEN 1 THEN
      BEGIN
        CALL SP_UpdateFriendRequest(_iUID, _iFUID);
      END;

    WHEN 2 THEN
      BEGIN
        CALL SP_UpdateFriendRemove(_iUID, _iFUID);
      END;

    WHEN 3 THEN
      BEGIN
        CALL SP_UpdateFriendRefuse(_iUID, _iFUID);
      END;

    WHEN 4 THEN
      BEGIN
        CALL SP_UpdateFriendAccept(_iUID, _iFUID);
      END;

    WHEN 5 THEN
      BEGIN
        CALL SP_UpdateFriendRemove(_iUID, _iFUID);
      END;

    ELSE
      BEGIN
        SELECT 2;
      END;
  END CASE;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateFriendRequest`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateFriendRequest`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iFUID` BIGINT(20) UNSIGNED
)
BEGIN
  DECLARE tableNo1 TINYINT(3) UNSIGNED;
  DECLARE tableNo2 TINYINT(3) UNSIGNED;
  DECLARE cnt INT(10) UNSIGNED;

  SET tableNo1 = _iFUID % 5;
  SET tableNo2 = _iUID % 5;
  SET cnt = 0;

  CASE tableNo1
    WHEN 0 THEN
      BEGIN
        SELECT COUNT(*) INTO cnt FROM player_friend_00 WHERE iUID = _iFUID AND iStatus = 1;
        IF cnt >= 50 THEN
          SELECT 1;
          BEGIN END;
        END IF;

        SELECT COUNT(*) INTO cnt FROM player_friend_00 WHERE iUID = _iFUID AND iStatus = 4;
        IF cnt >= 20 THEN
          SELECT 2;
          BEGIN END;
        END IF;

        INSERT INTO player_friend_00(iUID, iFUID, iStatus, tUpdated) VALUES(_iFUID, _iUID, 4, NOW());
      END;

    WHEN 1 THEN
      BEGIN
        SELECT COUNT(*) INTO cnt FROM player_friend_01 WHERE iUID = _iFUID AND iStatus = 1;
        IF cnt >= 50 THEN
          SELECT 1;
          BEGIN END;
        END IF;

        SELECT COUNT(*) INTO cnt FROM player_friend_01 WHERE iUID = _iFUID AND iStatus = 4;
        IF cnt >= 20 THEN
          SELECT 2;
          BEGIN END;
        END IF;

        INSERT INTO player_friend_01(iUID, iFUID, iStatus, tUpdated) VALUES(_iFUID, _iUID, 4, NOW());
      END;

    WHEN 2 THEN
      BEGIN
        SELECT COUNT(*) INTO cnt FROM player_friend_02 WHERE iUID = _iFUID AND iStatus = 1;
        IF cnt >= 50 THEN
          SELECT 1;
          BEGIN END;
        END IF;

        SELECT COUNT(*) INTO cnt FROM player_friend_02 WHERE iUID = _iFUID AND iStatus = 4;
        IF cnt >= 20 THEN
          SELECT 2;
          BEGIN END;
        END IF;

        INSERT INTO player_friend_02(iUID, iFUID, iStatus, tUpdated) VALUES(_iFUID, _iUID, 4, NOW());
      END;

    WHEN 3 THEN
      BEGIN
        SELECT COUNT(*) INTO cnt FROM player_friend_03 WHERE iUID = _iFUID AND iStatus = 1;
        IF cnt >= 50 THEN
          SELECT 1;
          BEGIN END;
        END IF;

        SELECT COUNT(*) INTO cnt FROM player_friend_03 WHERE iUID = _iFUID AND iStatus = 4;
        IF cnt >= 20 THEN
          SELECT 2;
          BEGIN END;
        END IF;

        INSERT INTO player_friend_03(iUID, iFUID, iStatus, tUpdated) VALUES(_iFUID, _iUID, 4, NOW());
      END;

    WHEN 4 THEN
      BEGIN
        SELECT COUNT(*) INTO cnt FROM player_friend_04 WHERE iUID = _iFUID AND iStatus = 1;
        IF cnt >= 50 THEN
          SELECT 1;
          BEGIN END;
        END IF;

        SELECT COUNT(*) INTO cnt FROM player_friend_04 WHERE iUID = _iFUID AND iStatus = 4;
        IF cnt >= 20 THEN
          SELECT 2;
          BEGIN END;
        END IF;

        INSERT INTO player_friend_04(iUID, iFUID, iStatus, tUpdated) VALUES(_iFUID, _iUID, 4, NOW());
      END;
  END CASE;

  CASE tableNo2
    WHEN 0 THEN
      BEGIN
        INSERT INTO player_friend_00(iUID, iFUID, iStatus, tUpdated) VALUES(_iUID, _iFUID, 3, NOW());
      END;

    WHEN 1 THEN
      BEGIN
        INSERT INTO player_friend_01(iUID, iFUID, iStatus, tUpdated) VALUES(_iUID, _iFUID, 3, NOW());
      END;

    WHEN 2 THEN
      BEGIN
        INSERT INTO player_friend_02(iUID, iFUID, iStatus, tUpdated) VALUES(_iUID, _iFUID, 3, NOW());
      END;

    WHEN 3 THEN
      BEGIN
        INSERT INTO player_friend_03(iUID, iFUID, iStatus, tUpdated) VALUES(_iUID, _iFUID, 3, NOW());
      END;

    WHEN 4 THEN
      BEGIN
        INSERT INTO player_friend_04(iUID, iFUID, iStatus, tUpdated) VALUES(_iUID, _iFUID, 3, NOW());
      END;
  END CASE;

  SELECT 0;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateFriendRemove`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateFriendRemove`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iFUID` BIGINT(20) UNSIGNED
)
BEGIN
  DECLARE tableNo1 TINYINT(3) UNSIGNED;
  DECLARE tableNo2 TINYINT(3) UNSIGNED;

  SET tableNo1 = _iFUID % 5;
  SET tableNo2 = _iUID % 5;

  CASE tableNo1
    WHEN 0 THEN
      BEGIN
        DELETE FROM player_friend_00 WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 1 THEN
      BEGIN
        DELETE FROM player_friend_01 WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 2 THEN
      BEGIN
        DELETE FROM player_friend_02 WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 3 THEN
      BEGIN
        DELETE FROM player_friend_03 WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 4 THEN
      BEGIN
        DELETE FROM player_friend_04 WHERE iUID = _iFUID AND iFUID = _iUID;
      END;
  END CASE;

  CASE tableNo2
    WHEN 0 THEN
      BEGIN
        DELETE FROM player_friend_00 WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 1 THEN
      BEGIN
        DELETE FROM player_friend_01 WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 2 THEN
      BEGIN
        DELETE FROM player_friend_02 WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 3 THEN
      BEGIN
        DELETE FROM player_friend_03 WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 4 THEN
      BEGIN
        DELETE FROM player_friend_04 WHERE iUID = _iUID AND iFUID = _iFUID;
      END;
  END CASE;

  SELECT 0;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateFriendRefuse`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateFriendRefuse`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iFUID` BIGINT(20) UNSIGNED
)
BEGIN
  DECLARE tableNo1 TINYINT(3) UNSIGNED;
  DECLARE tableNo2 TINYINT(3) UNSIGNED;

  SET tableNo1 = _iFUID % 5;
  SET tableNo2 = _iUID % 5;

  CASE tableNo1
    WHEN 0 THEN
      BEGIN
        UPDATE player_friend_00 SET iStatus = 6, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 1 THEN
      BEGIN
        UPDATE player_friend_01 SET iStatus = 6, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 2 THEN
      BEGIN
        UPDATE player_friend_02 SET iStatus = 6, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 3 THEN
      BEGIN
        UPDATE player_friend_03 SET iStatus = 6, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 4 THEN
      BEGIN
        UPDATE player_friend_04 SET iStatus = 6, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;
  END CASE;

  CASE tableNo2
    WHEN 0 THEN
      BEGIN
        UPDATE player_friend_00 SET iStatus = 5, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 1 THEN
      BEGIN
        UPDATE player_friend_01 SET iStatus = 5, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 2 THEN
      BEGIN
        UPDATE player_friend_02 SET iStatus = 5, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 3 THEN
      BEGIN
        UPDATE player_friend_03 SET iStatus = 5, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 4 THEN
      BEGIN
        UPDATE player_friend_04 SET iStatus = 5, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;
  END CASE;

  SELECT 0;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_UpdateFriendAccept`;
DELIMITER //
CREATE PROCEDURE `SP_UpdateFriendAccept`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iFUID` BIGINT(20) UNSIGNED
)
BEGIN
  DECLARE tableNo1 TINYINT(3) UNSIGNED;
  DECLARE tableNo2 TINYINT(3) UNSIGNED;

  SET tableNo1 = _iFUID % 5;
  SET tableNo2 = _iUID % 5;

  CASE tableNo1
    WHEN 0 THEN
      BEGIN
        UPDATE player_friend_00 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 1 THEN
      BEGIN
        UPDATE player_friend_01 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 2 THEN
      BEGIN
        UPDATE player_friend_02 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 3 THEN
      BEGIN
        UPDATE player_friend_03 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;

    WHEN 4 THEN
      BEGIN
        UPDATE player_friend_04 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iFUID AND iFUID = _iUID;
      END;
  END CASE;

  CASE tableNo2
    WHEN 0 THEN
      BEGIN
        UPDATE player_friend_00 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 1 THEN
      BEGIN
        UPDATE player_friend_01 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 2 THEN
      BEGIN
        UPDATE player_friend_02 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 3 THEN
      BEGIN
        UPDATE player_friend_03 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;

    WHEN 4 THEN
      BEGIN
        UPDATE player_friend_04 SET iStatus = 1, tUpdated = NOW() WHERE iUID = _iUID AND iFUID = _iFUID;
      END;
  END CASE;

  SELECT 0;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_SelectPlayerPost`;
DELIMITER //
CREATE PROCEDURE `SP_SelectPlayerPost`(
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iSendType` TINYINT(3) UNSIGNED
)
BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;
  SET tableNo = _iUID % 5;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        UPDATE player_post_00 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_00 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 LIMIT 50;
      END;

	WHEN 1 THEN
      BEGIN
        UPDATE player_post_01 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_01 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 LIMIT 50;
      END;

	WHEN 2 THEN
      BEGIN
        UPDATE player_post_02 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_02 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 LIMIT 50;
      END;

	WHEN 3 THEN
      BEGIN
        UPDATE player_post_03 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_03 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 LIMIT 50;
      END;

	WHEN 4 THEN
      BEGIN
        UPDATE player_post_04 SET iStatus = 2, tUpdated = NOW() WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 AND UNIX_TIMESTAMP(tExpire) - UNIX_TIMESTAMP(tCreated) > 259200;
        SELECT iPostIdx, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, UNIX_TIMESTAMP(tExpire), UNIX_TIMESTAMP(tCreated) FROM player_post_04 FORCE INDEX(`index_uid`) WHERE iUID = _iUID AND iSendType = _iSendType AND iStatus = 0 LIMIT 50;
      END;
  END CASE;
END//
DELIMITER ;

DROP PROCEDURE IF EXISTS `SP_InsertPost`;
DELIMITER //
CREATE PROCEDURE `SP_InsertPost`(
  IN `_iFUID` BIGINT(20) UNSIGNED,
  IN `_iUID` BIGINT(20) UNSIGNED,
  IN `_iSendType` TINYINT(3) UNSIGNED,
  IN `_iStatus` TINYINT(3) UNSIGNED,
  IN `_iPostType` TINYINT(3) UNSIGNED,
  IN `_iValue1` INT(10) UNSIGNED,
  IN `_iValue2` INT(10) UNSIGNED,
  IN `_tExpire` INT(10) UNSIGNED,
  IN `_tCurrent` INT(10) UNSIGNED
)
BEGIN
  DECLARE tableNo TINYINT(3) ZEROFILL;
  DECLARE last_insert BIGINT(20);
  SET tableNo = _iFUID % 5;
  SET last_insert = 0;

  CASE tableNo
    WHEN 0 THEN
      BEGIN
        SELECT COUNT(*) INTO last_insert FROM player_post_00 WHERE iUID = _iFUID;

        INSERT INTO player_post_00(iPostIdx, iUID, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, tExpire, tCreated, tUpdated) VALUES(
                                   last_insert + 1, _iFUID, _iUID, _iSendType, _iStatus, _iPostType, _iValue1, _iValue2, FROM_UNIXTIME(_tExpire), FROM_UNIXTIME(_tCurrent), NOW());
      END;

	WHEN 1 THEN
      BEGIN
        SELECT COUNT(*) INTO last_insert FROM player_post_01 WHERE iUID = _iFUID;

        INSERT INTO player_post_01(iPostIdx, iUID, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, tExpire, tCreated, tUpdated) VALUES(
                                   last_insert + 1, _iFUID, _iUID, _iSendType, _iStatus, _iPostType, _iValue1, _iValue2, FROM_UNIXTIME(_tExpire), FROM_UNIXTIME(_tCurrent), NOW());
      END;

	WHEN 2 THEN
      BEGIN
        SELECT COUNT(*) INTO last_insert FROM player_post_02 WHERE iUID = _iFUID;

        INSERT INTO player_post_02(iPostIdx, iUID, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, tExpire, tCreated, tUpdated) VALUES(
                                   last_insert + 1, _iFUID, _iUID, _iSendType, _iStatus, _iPostType, _iValue1, _iValue2, FROM_UNIXTIME(_tExpire), FROM_UNIXTIME(_tCurrent), NOW());
      END;

	WHEN 3 THEN
      BEGIN
        SELECT COUNT(*) INTO last_insert FROM player_post_03 WHERE iUID = _iFUID;

        INSERT INTO player_post_03(iPostIdx, iUID, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, tExpire, tCreated, tUpdated) VALUES(
                                   last_insert + 1, _iFUID, _iUID, _iSendType, _iStatus, _iPostType, _iValue1, _iValue2, FROM_UNIXTIME(_tExpire), FROM_UNIXTIME(_tCurrent), NOW());
      END;

	WHEN 4 THEN
      BEGIN
        SELECT COUNT(*) INTO last_insert FROM player_post_04 WHERE iUID = _iFUID;

        INSERT INTO player_post_04(iPostIdx, iUID, iFUID, iSendType, iStatus, iPostType, iValue1, iValue2, tExpire, tCreated, tUpdated) VALUES(
                                   last_insert + 1, _iFUID, _iUID, _iSendType, _iStatus, _iPostType, _iValue1, _iValue2, FROM_UNIXTIME(_tExpire), FROM_UNIXTIME(_tCurrent), NOW());
      END;
  END CASE;
END//
DELIMITER ;
