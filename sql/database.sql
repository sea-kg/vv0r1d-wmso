
CREATE TABLE `wwm_users` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `pass` varchar(255) NOT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `wwm_map` (
  `pos_x` BIGINT NOT NULL,
  `pos_y` BIGINT NOT NULL,
  `can_move` varchar(1) NOT NULL,
  `bg` INT NOT NULL,
  `fr` INT NOT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `wwm_map`
  ADD UNIQUE KEY `uniq_pos_xy` (`pos_x`, `pos_y`);