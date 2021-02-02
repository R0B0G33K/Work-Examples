'use strict';

module.exports = {
  up: async (queryInterface, Sequelize) => {
    return queryInterface.createTable(
      'bookings',{
          keyNum: {
              type: Sequelize.INTEGER,
              primaryKey: true
          },
          personID:{
              type: Sequelize.INTEGER,
              primaryKey: true
          },
          predictedTime:{
              type: Sequelize.DATE
          },
          returnedTime:{
              type: Sequelize.DATE
          },
          reason: {
              type: Sequelize.STRING
          },
          attendent: {
              type: Sequelize.STRING
          },
          attendent2: {
              type: Sequelize.STRING
          },
          createdAt: {
              allowNull: false,
              type: Sequelize.DATE,
              primaryKey: true
          },
          updatedAt: {
            type: Sequelize.DATE
          }
      })
  },

  down: async (queryInterface, Sequelize) => {
    await queryInterface.dropTable('bookings');
  }
};
