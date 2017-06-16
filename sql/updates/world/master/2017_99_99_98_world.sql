-- Spell Proc Event



-- 35110 Bombardment
DELETE FROM `spell_proc_event` WHERE `entry`=35110;
INSERT INTO `spell_proc_event` (`entry`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`spellFamilyMask3`,`procFlags`,`procEx`,`ppmRate`,`CustomChance`,`Cooldown`) VALUES
(35110, 1, 9, 4096, 0, 0, 0, 0, 2, 0, 0, 0);

-- 199754 Riposte
DELETE FROM `spell_proc_event` WHERE `entry`=199754;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `spellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES(199754,0,0,0,0,0,0,28,32,0,0,0);
