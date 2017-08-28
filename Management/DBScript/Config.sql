DROP TABLE IF EXISTS `server_config`;
CREATE TABLE  `server_config` (
  `seq` int(3) unsigned NOT NULL,
  `world` varchar(30) NOT NULL,
  `sid` int(10) unsigned NOT NULL,
  `section` varchar(30) NOT NULL,
  `name` varchar(50) NOT NULL,
  `value` varchar(256) NOT NULL,
  `comment` varchar(256) NOT NULL,
  PRIMARY KEY (`world`,`sid`,`section`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DELIMITER $$

DROP PROCEDURE IF EXISTS `SP_SelectServerConfig` $$
CREATE PROCEDURE `SP_SelectServerConfig`(
  _world VARCHAR(30),
  _sid INT(10),
  _section VARCHAR(30)
)
BEGIN


  CASE _section
    WHEN ''     THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section = 'MstS'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 4 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section = 'ChnS'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 5 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'GtwS%'
        ORDER BY SO, world, section, seq
        ;
      END;

    WHEN 'ClientPort' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'GtwS%' AND name in ( 'listen_addr', 'listen_port' )
        ORDER BY SO, sWorld, sSection, iSeq
        ;
      END;

    WHEN 'ServerPort' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%' AND name in ( 'remote_addr', 'remote_port', 'provider_name' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'ChnS%' AND name in ( 'listen_addr', 'listen_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 4 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'MstS%' AND name in ( 'listen_addr', 'listen_port' )
        ORDER BY SO, sWorld, sSection, iSeq
        ;
      END;

    WHEN 'PAgent' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'SAgent%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, SUBSTR(section, 1, 6) as section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'PAgent%'
        ORDER BY SO, section, seq
        ;
      END;
      
	WHEN 'SAgent' THEN
      BEGIN
        SELECT	world, section, name, value, sid, seq, 1 as SO
        FROM	SERVER_CONFIG
        WHERE	world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT	world, SUBSTR(section, 1, 6) as section, name, value, sid, seq, 2 as SO
        FROM	SERVER_CONFIG
        WHERE	world = _world AND sid = _sid AND section like 'SAgent%'
        UNION ALL
        SELECT	a.world, a.section, a.name, a.value, a.sid, a.seq, 3 as SO
        FROM	SERVER_CONFIG a,
        	(
			SELECT value as cmp_value
			FROM   SERVER_CONFIG
			WHERE  world = _world AND sid = _sid AND section like 'SAgent%'
			AND 	 name = 'managed_pagent' LIMIT 1
		) b
        WHERE 	LOCATE(a.section, b.cmp_value) > 0
        ORDER BY SO, section, seq
        ;
      END;

    WHEN 'LogS' THEN
      BEGIN
        SELECT world, SUBSTR(section, 1, 4) as section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'LogS%'
        ORDER BY SO, section, seq
        ;
      END;

    WHEN 'Dbgw' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, SUBSTR(section, 1, 4) as section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'Dbgw%'
        ORDER BY SO, section, seq
        ;
      END;

    WHEN 'GtwS' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section = 'MstS' AND name in ( 'id', 'listen_addr', 'listen_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 4 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section = 'ChnS' AND name in ( 'id', 'listen_addr', 'listen_port' )
        UNION ALL
        SELECT world, SUBSTR(section, 1, 4) as section, name, value, sid, seq, 5 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'GtwS%'
        ORDER BY SO, section, seq
        ;
      END;

    WHEN 'MstS' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, SUBSTR(section, 1, 4) as section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'MstS%'
        ORDER BY SO, section, seq
        ;
      END;

    WHEN 'ChnS' THEN
      BEGIN
        SELECT world, section, name, value, sid, seq, 1 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'DB%'
        UNION ALL
        SELECT world, section, name, value, sid, seq, 2 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section like 'LogS%' AND name in ( 'id', 'bind_addr', 'bind_port' )
        UNION ALL
        SELECT world, section, name, value, sid, seq, 3 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND section = 'MstS' AND name in ( 'id', 'listen_addr', 'listen_port' )
        UNION ALL
        SELECT world, SUBSTR(section, 1, 4) as section, name, value, sid, seq, 4 as SO
        FROM   SERVER_CONFIG
        WHERE  world = _world AND sid = _sid AND section like 'ChnS%'
        ORDER BY SO, section, seq
        ;
     END;
  END CASE;
END $$

DELIMITER ;


 DELIMITER $$

DROP PROCEDURE IF EXISTS `SP_CreateServerConfig` $$
CREATE PROCEDURE `SP_CreateServerConfig`(
  _seq INT(3),
  _world VARCHAR(30),
  _sid INT(10),
  _section VARCHAR(30),
  _name VARCHAR(256),
  _value VARCHAR(256),
  _comment VARCHAR(256)
)
BEGIN

    DECLARE CNT INT;
    SET CNT = 0;

    SELECT COUNT(*) INTO CNT FROM SERVER_CONFIG
    WHERE  world = _world AND sid = _sid AND section = _section AND name = _name;

    IF CNT > 0 THEN
      BEGIN
        UPDATE SERVER_CONFIG SET value = _value, comment = _comment
       WHERE  world = _world AND sid = _sid AND section = _section AND name = _name;
      END;
    ELSE
      BEGIN
        IF _seq >= 0 THEN
          BEGIN
            INSERT INTO SERVER_CONFIG VALUES
            ( _seq, _world, _sid, _section, _name, _value, _comment );
          END;
        ELSE
          BEGIN
            INSERT INTO SERVER_CONFIG
            SELECT  IFNULL(MAX(seq), 0)+1, _world, _sid, _section, _name, _value, _comment
            FROM    SERVER_CONFIG
            WHERE   world = _world AND sid = _sid;
          END;
        END IF;
      END;
    END IF;

    SELECT 1;

END $$

DELIMITER ;